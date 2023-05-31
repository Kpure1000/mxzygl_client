#include "bvh.h"

#include <cmath>

res::BoneMesh::BoneMesh(size_t nBones)
{
    size_t nVert = 16;
    size_t nIndi = 6 * 3;
    vertices.reserve(nBones * nVert);
    normals.reserve(nBones * nVert);

    for (size_t i = 0; i < 4; i++) {
        trans[i].reserve(nBones * nVert);
    }
    for (size_t i = 0; i < 3; i++) {
        trans_nor[i].reserve(nBones * nVert);
    }
    indices.reserve(nBones * nIndi);
}

res::BoneMesh::BoneMesh(const QMatrix4x4 &trans_source, const QMatrix4x4 &trans_target)
{
    float width = 0.2f; // half width
    float height = 2.0f;
    float inv_magnitude = 1.0f / std::sqrt(width * width + height * height);
    float nor_w = width * inv_magnitude;
    float nor_h = height * inv_magnitude;

    auto pos_source = trans_source * QVector3D{.0f,.0f,.0f};
    auto pos_target = trans_target * QVector3D{.0f,.0f,.0f};
    auto direction = pos_target - pos_source;
    auto position = pos_source;
    auto rotation = QQuaternion::rotationTo({.0f, 1.f, .0f}, direction);
    auto scale = direction.length() / height;
    QMatrix4x4 trans_mat;
    trans_mat.translate(position);
    trans_mat.rotate(rotation);
    trans_mat.scale(scale);
    auto trans_mat_nor = trans_mat.normalMatrix();

    //cone
    vertices = {
        { width,  0.0f  ,  width}, // 0
        { width,  0.0f  , -width}, // 1
        {-width,  0.0f  , -width}, // 2
        {-width,  0.0f  ,  width}, // 3

        { width,  0.0f  ,  width}, // 0+4
        { width,  0.0f  , -width}, // 1+4
        { 0.0f ,  height,  0.0f }, // 6

        { width,  0.0f  , -width}, // 1+6
        {-width,  0.0f  , -width}, // 2+6
        { 0.0f ,  height,  0.0f }, // 9

        {-width,  0.0f  , -width}, // 2+8
        {-width,  0.0f  ,  width}, // 3+8
        { 0.0f ,  height,  0.0f }, // 12

        {-width,  0.0f  ,  width}, // 3+10
        { width,  0.0f  ,  width}, // 0+4+10
        { 0.0f ,  height,  0.0f }, // 15
    };
    normals = {
        { 0.0f , -1.0f ,  0.0f },
        { 0.0f , -1.0f ,  0.0f },
        { 0.0f , -1.0f ,  0.0f },
        { 0.0f , -1.0f ,  0.0f },

        { nor_h,  nor_w,  0.0f },
        { nor_h,  nor_w,  0.0f },
        { nor_h,  nor_w,  0.0f },

        { 0.0f ,  nor_w, -nor_h},
        { 0.0f ,  nor_w, -nor_h},
        { 0.0f ,  nor_w, -nor_h},

        {-nor_h,  nor_w,  0.0f },
        {-nor_h,  nor_w,  0.0f },
        {-nor_h,  nor_w,  0.0f },

        { 0.0f ,  nor_w,  nor_h},
        { 0.0f ,  nor_w,  nor_h},
        { 0.0f ,  nor_w,  nor_h},
    };
    indices = {
        0 , 2 , 1 ,
        0 , 3 , 2 ,
        4 , 5 , 6 ,
        7 , 8 , 9 ,
        10, 11, 12,
        13, 14, 15,
    };

    for (size_t i = 0; i < 4; i++) {
        trans[i].resize(vertices.size(), trans_mat.column(i));
    }

    for (size_t i = 0; i < 3; i++) {
        trans_nor[i].resize(vertices.size(), {trans_mat_nor.data()[0 + i], trans_mat_nor.data()[3 + i], trans_mat_nor.data()[6 + i]});
    }

}

void res::BoneMesh::operator<<(const BoneMesh &boneMesh)
{
    auto nVert = vertices.size();
    for (size_t i = 0; i < boneMesh.indices.size(); i++) {
        indices.emplace_back(boneMesh.indices[i] + nVert);
    }
    for (size_t i = 0; i < boneMesh.vertices.size(); i++) {
        vertices.emplace_back(boneMesh.vertices[i]);
        normals.emplace_back(boneMesh.normals[i]);
        for (size_t col = 0; col < 4; col++) {
            trans[col].emplace_back(boneMesh.trans[col][i]);
        }
        for (size_t col = 0; col < 3; col++) {
            trans_nor[col].emplace_back(boneMesh.trans_nor[col][i]);
        }
    }
}

