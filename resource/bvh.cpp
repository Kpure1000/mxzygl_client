#include "bvh.h"

#include <cmath>

res::BoneMesh::BoneMesh(size_t nBones)
{
    size_t nVert = 16;
    size_t nIndi = 6 * 3;
    vertices.reserve(nBones * nVert);
    normals.reserve(nBones * nVert);
    trans_r0.reserve(nBones * nVert);
    trans_r1.reserve(nBones * nVert);
    trans_r2.reserve(nBones * nVert);
    trans_r3.reserve(nBones * nVert);
    trans_nor_r0.reserve(nBones * nVert);
    trans_nor_r1.reserve(nBones * nVert);
    trans_nor_r2.reserve(nBones * nVert);
//    trans.reserve(nBones * nVert);
//    trans_nor.reserve(nBones * nVert);
    indices.reserve(nBones * nIndi);
}

res::BoneMesh::BoneMesh(const QMatrix4x4 &trans_mat, const QMatrix3x3 &trans_nor)
{
    float width = 0.5f; // half width
    float height = 2.0f;
    float inv_magnitude = 1.0f / std::sqrt(width * width + height * height);
    float nor_w = width * inv_magnitude;
    float nor_h = height * inv_magnitude;
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
//    trans.resize(vertices.size(), trans_mat);
//    this->trans_nor.resize(vertices.size(), trans_nor);
    trans_r0.resize(vertices.size(), trans_mat.row(0));
    trans_r1.resize(vertices.size(), trans_mat.row(1));
    trans_r2.resize(vertices.size(), trans_mat.row(2));
    trans_r3.resize(vertices.size(), trans_mat.row(3));
    trans_nor_r0.resize(vertices.size(), {trans_nor.data()[0], trans_nor.data()[1], trans_nor.data()[2]});
    trans_nor_r1.resize(vertices.size(), {trans_nor.data()[3], trans_nor.data()[4], trans_nor.data()[5]});
    trans_nor_r2.resize(vertices.size(), {trans_nor.data()[6], trans_nor.data()[7], trans_nor.data()[8]});
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
        trans_r0.emplace_back(boneMesh.trans_r0[i]);
        trans_r1.emplace_back(boneMesh.trans_r1[i]);
        trans_r2.emplace_back(boneMesh.trans_r2[i]);
        trans_r3.emplace_back(boneMesh.trans_r3[i]);
        trans_nor_r0.emplace_back(boneMesh.trans_nor_r0[i]);
        trans_nor_r1.emplace_back(boneMesh.trans_nor_r1[i]);
        trans_nor_r2.emplace_back(boneMesh.trans_nor_r2[i]);
//        trans.emplace_back(boneMesh.trans[i]);
//        trans_nor.emplace_back(boneMesh.trans_nor[i]);
    }
}

