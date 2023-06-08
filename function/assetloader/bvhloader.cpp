#include "bvhloader.h"

#include <cmath>
#include <QMatrix4x4>
#include <QTransform>
#include <QFileInfo>
#include <QDebug>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "resource/bvh.h"
#include "utils/jobsystem.h"

BVHLoader::BVHLoader(QObject *parent) : AssetCache(parent)
{
    connect(this, &AssetCache::onCacheTimeout, this, [this](const QString& filePath) {
        auto filePath_std = filePath.toStdString();
        auto model_asset = BVHManager::getInstance()->get(filePath_std);
        if (model_asset.use_count() <= 2) {
            BVHManager::getInstance()->remove(filePath_std);
            this->removeCache(filePath);
        } else {
            this->restartCache(filePath);
        }
    });
}

void readNode(aiNode *node, const std::function<void(aiNode *)> &processor)
{
    // 先序遍历
    processor(node);
    for (size_t i = 0; i < node->mNumChildren; i++) {
        readNode(node->mChildren[i], processor);
    }
}

void readBone(res::Bone *bone, const std::function<void(res::Bone *)> &proc)
{
    proc(bone);
    for (auto child : bone->children) {
        readBone(child, proc);
    }
}

aiBone* findChildBone(aiBone *bone, std::unordered_map<std::string, aiNode *> &node_map, std::unordered_map<std::string, aiBone *> &bone_map)
{
    if (node_map.find(bone->mName.C_Str()) != node_map.end()) {
        auto child_node = node_map[bone->mName.C_Str()];
        for (size_t k = 0; k < child_node->mNumChildren; k++) {
            auto child_name = child_node->mChildren[k]->mName.C_Str();
            if (bone_map.find(child_name) != bone_map.end()) {
                return bone_map[child_name];
            }
        }
    }
    return nullptr;
}

std::shared_ptr<res::BVH> BVHLoader::loadBVH(const QString &filePath)
{
    Assimp::Importer importer;
    auto importFlag = aiProcessPreset_TargetRealtime_Fast;

    auto scene = importer.ReadFile(filePath.toStdString(), importFlag);

    if (!scene) {
        qDebug() << "BVHLoader>> Assimp Scene" << filePath << "Load Failed";
        return nullptr;
    }

    if (!scene->HasMeshes()) {
        qDebug() << "BVHLoader>> Assimp Scene" << filePath << "DONNT Have Any Meshes";
        return nullptr;
    }

    auto bvh = std::make_shared<res::BVH>();

    std::unordered_map<std::string, aiNode *> node_map;
    std::unordered_map<std::string, aiNodeAnim *> joint_map;
    std::unordered_map<std::string, std::shared_ptr<res::Bone>> bone_map;

    readNode(scene->mRootNode, [=, &node_map](aiNode *node){
        node_map.insert({node->mName.C_Str(), node});
    });

    unsigned int nFrames = 0;

    if (scene->mNumAnimations < 1) {
        qDebug("BVHLoader>> Assimp Scene Don't Have Any Animation Data.");
        return nullptr;
    }

    auto animation = scene->mAnimations[0];

//    qDebug("Duration=%lf, Ticks=%lf", animation->mDuration, animation->mTicksPerSecond);

    for (size_t j = 0; j < animation->mNumChannels; j++) {
        auto channel = animation->mChannels[j];
        joint_map.insert({channel->mNodeName.C_Str(), channel});
        nFrames = std::max(nFrames, channel->mNumPositionKeys);
        nFrames = std::max(nFrames, channel->mNumRotationKeys);
        nFrames = std::max(nFrames, channel->mNumScalingKeys);
    }

    bvh->ssp = animation->mTicksPerSecond;
    bvh->nFrames = nFrames;

    res::Bone *rootBone = nullptr;

    QVector3D pmin{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    QVector3D pmax{std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};

    readNode(scene->mRootNode, [=, &pmin, &pmax, &rootBone, &joint_map, &bone_map](aiNode *node) {
        // If the node is marked as necessary, copy it into the skeleton and check its children.
        // If the node is marked as not necessary, skip it and do not iterate over its children.
        if (joint_map.find(node->mName.C_Str()) != joint_map.end()) {
            auto cur_joint = joint_map[node->mName.C_Str()];
            auto bone = std::make_shared<res::Bone>();
            if (!rootBone)
                rootBone = bone.get();
            bone_map.insert({node->mName.C_Str(), bone});

            aiNode *parent = node->mParent;
            while (parent) {
                // Find parent joint
                if (bone_map.find(parent->mName.C_Str()) != bone_map.end()) {
                    break;
                }
                parent = parent->mParent;
            }
            std::shared_ptr<res::Bone> bone_parent = parent ? bone_map[parent->mName.C_Str()] : nullptr;
            if (bone_parent)
                bone_parent->children.emplace_back(bone.get());

            for (unsigned int k = 0; k < nFrames; k++) {
                unsigned int pk = std::min(k, cur_joint->mNumPositionKeys - 1);
                QVector3D position {cur_joint->mPositionKeys[pk].mValue.x, cur_joint->mPositionKeys[pk].mValue.y, cur_joint->mPositionKeys[pk].mValue.z};
                unsigned int rk = std::min(k, cur_joint->mNumRotationKeys - 1);
                QQuaternion rotation {cur_joint->mRotationKeys[rk].mValue.w, cur_joint->mRotationKeys[rk].mValue.x, cur_joint->mRotationKeys[rk].mValue.y,cur_joint->mRotationKeys[rk].mValue.z};
                unsigned int sk = std::min(k, cur_joint->mNumScalingKeys - 1);
                QVector3D scale    {cur_joint->mScalingKeys[sk].mValue.x,  cur_joint->mScalingKeys[sk].mValue.y,  cur_joint->mScalingKeys[sk].mValue.z};

                QMatrix4x4 trans;
                trans.translate(position);
                trans.rotate(rotation);
                trans.scale(scale);
                if (nullptr != bone_parent)
                    trans = bone_parent->trans_mats[k] * trans;
                bone->trans_mats.emplace_back(trans);

                QVector3D pos{trans.data()[12],trans.data()[13],trans.data()[14]};
                pmin.setX(std::min(pos.x(), pmin.x()));
                pmin.setY(std::min(pos.y(), pmin.y()));
                pmin.setZ(std::min(pos.z(), pmin.z()));
                pmax.setX(std::max(pos.x(), pmax.x()));
                pmax.setY(std::max(pos.y(), pmax.y()));
                pmax.setZ(std::max(pos.z(), pmax.z()));

            }
        }
    });

    QVector3D centroid = (pmin + pmax) * 0.5f;
    float inv_semi_diagonal = 1.f / ((pmax - pmin).length() * 0.5f);
    QMatrix4x4 obj2org;
    obj2org.scale({inv_semi_diagonal, inv_semi_diagonal, inv_semi_diagonal});
    obj2org.translate(-centroid);
    obj2org.rotate({.0f, .0f, .0f, .0f});

    for (size_t k = 0; k < nFrames; k++) {
        auto boneMesh = std::make_shared<res::BoneMesh>(bone_map.size());
        readBone(rootBone, [=](res::Bone *bone) {
            for (auto child : bone->children) {
                (*boneMesh) << res::BoneMesh(obj2org * bone->trans_mats[k], obj2org * child->trans_mats[k]);
            }
        });
        bvh->boneMeshes.emplace_back(boneMesh);
    }

    return bvh;
}

void BVHLoader::cachedAsyncLoad(const QString &filePath, std::function<void (bool)> loadCallBack)
{
    if (!BVHManager::getInstance()->has(filePath.toStdString())) {
        JobSystem::getInstance()->submit([filePath, loadCallBack, this]() {
            auto fileExt = filePath.split('.').back();
            std::shared_ptr<res::BVH> bvh;
            if (fileExt == "bvh") {
                bvh = BVHLoader::getInstance()->loadBVH(filePath);
            } else {
                qDebug() << "BVHLoader::cachedAsyncLoad>> Unsupported BVH" << filePath << "(.bvh is Legal)";
                loadCallBack(false);
                return;
            }
            if (bvh == nullptr) {
                qDebug() << "BVHLoader::cachedAsyncLoad>> BVH" << filePath << "Load Failed";
                loadCallBack(false);
                return;
            }
            BVHManager::getInstance()->add(filePath.toStdString(), bvh);
            emit onAssetLoaded(filePath);
            loadCallBack(true);
        });
    } else {
        emit onAssetLoaded(filePath);
        loadCallBack(true);
    }
}

void BVHLoader::tempAsyncLoad(const QString &filePath, std::function<void (std::shared_ptr<res::BVH>)> loadCallBack)
{
    JobSystem::getInstance()->submit([filePath, loadCallBack]() {
        auto fileExt = filePath.split('.').back();
        std::shared_ptr<res::BVH> model = nullptr;
        if (fileExt == "bvh") {
            model = BVHLoader::getInstance()->loadBVH(filePath);
        } else {
            qDebug() << "BVHLoader::tempAsyncLoad>> Unsupported BVH" << filePath << "(.bvh is Legal)";
        }
        if (model == nullptr) {
            qDebug() << "BVHLoader::tempAsyncLoad>> BVH" << filePath << "Load Failed";
        }
        loadCallBack(model);
    });
}

