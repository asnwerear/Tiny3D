﻿/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2019  Answer Wong
 * For latest info, see https://github.com/asnwerear/Tiny3D
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/


#include "SceneGraph/T3DSGSphere.h"
#include "Render/T3DRenderQueue.h"
#include "Render/T3DHardwareBufferManager.h"
#include "Render/T3DHardwareVertexBuffer.h"
#include "Render/T3DHardwareIndexBuffer.h"
#include "Render/T3DVertexArrayObject.h"
#include "Bound/T3DSphereBound.h"


namespace Tiny3D
{
    //--------------------------------------------------------------------------

    struct SphereVertex
    {
        SphereVertex()
            : position(Vector3::ZERO)
            , color(Color4::WHITE.A8R8G8B8())
        {

        }

        Vector3     position;
        uint32_t    color;
    };

    //--------------------------------------------------------------------------

    const size_t MAX_STACKS = 18;
    const size_t MAX_SLICES = 18;
    const size_t MAX_VERTICES = (MAX_STACKS + 1) * (MAX_SLICES + 1);
    const size_t MAX_TRIANGLES = MAX_STACKS * MAX_SLICES * 2;

    //--------------------------------------------------------------------------

    SGSpherePtr SGSphere::create(const Vector3 &center, Real radius, 
        ID uID /* = E_NID_AUTOMATIC */)
    {
        SGSpherePtr sphere = new SGSphere(uID);
        sphere->release();

        if (sphere->init(center, radius) != T3D_OK)
        {
            sphere = nullptr;
        }

        return sphere;
    }

    //--------------------------------------------------------------------------

    SGSphere::SGSphere(ID uID /* = E_NID_AUTOMATIC */)
        : SGRenderable(uID)
        , mCenter(Vector3::ZERO)
        , mRadius(REAL_ONE)
        , mVAO(nullptr)
        , mBound(nullptr)
    {

    }

    //--------------------------------------------------------------------------

    SGSphere::~SGSphere()
    {

    }

    //--------------------------------------------------------------------------

    Node::Type SGSphere::getNodeType() const
    {
        return E_NT_SPHERE;
    }

    //--------------------------------------------------------------------------

    TResult SGSphere::init(const Vector3 &center, Real radius)
    {
        TResult ret = T3D_OK;

        mCenter = center;
        mRadius = radius;

        SphereVertex vertices[MAX_VERTICES];
        const size_t MAX_INDICES = MAX_TRIANGLES * 3;
        uint16_t indices[MAX_INDICES];

        setupSphere(vertices, MAX_VERTICES, indices, MAX_INDICES);

        do 
        {
            mVAO = T3D_HARDWARE_BUFFER_MGR.createVertexArrayObject(true);
            if (mVAO == nullptr)
            {
                ret = T3D_ERR_INVALID_POINTER;
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Create VAO for SGSphere \
                    failed !");
                break;
            }

            ret = mVAO->beginBinding();
            if (ret != T3D_OK)
            {
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Binding VAO for SGSphere \
                    failed !");
                break;
            }

            // 创建顶点声明
            VertexDeclarationPtr decl
                = T3D_HARDWARE_BUFFER_MGR.createVertexDeclaration();
            if (decl == nullptr)
            {
                ret = T3D_ERR_INVALID_POINTER;
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Create vertex declaration for \
                    SGSphere failed !");
                break;
            }

            decl->addAttribute(VertexAttribute(0, 0,
                VertexAttribute::E_VAT_FLOAT3,
                VertexAttribute::E_VAS_POSITION));
            decl->addAttribute(VertexAttribute(0, sizeof(Vector3),
                VertexAttribute::E_VAT_COLOR,
                VertexAttribute::E_VAS_DIFFUSE));

            // 创建VBO
            HardwareVertexBufferPtr vbo
                = T3D_HARDWARE_BUFFER_MGR.createVertexBuffer(
                    sizeof(SphereVertex), MAX_VERTICES,
                    HardwareVertexBuffer::E_HBU_STATIC, false, false);
            if (vbo == nullptr)
            {
                ret = T3D_ERR_INVALID_POINTER;
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Create vertex buffer for \
                    SGSphere failed !");
                break;
            }

            // 写顶点数据
            size_t size = sizeof(SphereVertex) * MAX_VERTICES;
            size_t bytesOfWritten = vbo->writeData(0, size, vertices);
            if (bytesOfWritten != size)
            {
                ret = T3D_ERR_HW_BUFFER_WRITE;
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Write vertices data for SGSphere \
                    failed !");
                break;
            }

            // 创建IBO
            HardwareIndexBufferPtr ibo
                = T3D_HARDWARE_BUFFER_MGR.createIndexBuffer(
                    HardwareIndexBuffer::E_IT_16BITS, MAX_INDICES,
                    HardwareIndexBuffer::E_HBU_STATIC, false, false);
            if (ibo == nullptr)
            {
                ret = T3D_ERR_INVALID_POINTER;
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Create index buffer for SGSphere \
                    failed !");
                break;
            }

            // 写索引数据
            size = sizeof(uint16_t) * MAX_INDICES;
            bytesOfWritten = ibo->writeData(0, size, indices);
            if (bytesOfWritten != size)
            {
                ret = T3D_ERR_HW_BUFFER_WRITE;
                T3D_LOG_ERROR(LOG_TAG_SCENE, "Write indices data for SGSphere \
                    failed !");
                break;
            }

            mVAO->setVertexDeclaration(decl);
            mVAO->addVertexBuffer(vbo);
            mVAO->setIndexBuffer(ibo);
            mVAO->setPrimitiveType(Renderer::E_PT_TRIANGLE_LIST);

            mVAO->endBinding();

            // 构建碰撞体
            mBound = SphereBound::create(this);
            mBound->setParams(mCenter, mRadius);

            // 需要刷新碰撞体的世界变换
            setDirty(true);
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    void SGSphere::setupSphere(void *vertices, size_t vertexCount,
        uint16_t *indices, size_t indexCount)
    {
        SphereVertex *vert = (SphereVertex *)vertices;

        uint32_t color = Color4::WHITE.A8R8G8B8();

        Radian alphaStep(Real(2.0) * Math::PI / MAX_STACKS);
        Radian betaStep(Math::PI / MAX_SLICES);
        Radian alpha(0.0);
        Radian beta;

        int32_t i = 0, j = 0, idx = 0;

        for (i = 0; i <= MAX_STACKS; ++i)
        {
            Real sinAlpha = Math::sin(alpha);
            Real cosAlpha = Math::cos(alpha);
            beta = -Math::PI * REAL_HALF;

            for (j = 0; j <= MAX_SLICES; ++j, ++idx)
            {
                Real sinBeta = Math::sin(beta);
                Real cosBeta = Math::cos(beta);

                vert[idx].position[0] 
                    = mCenter[0] + mRadius * cosBeta * cosAlpha;
                vert[idx].position[1] 
                    = mCenter[1] + mRadius * sinBeta;
                vert[idx].position[2] 
                    = mCenter[2] + mRadius * cosBeta * sinAlpha;

                vert[idx].color = color;

                beta += betaStep;
            }

            alpha += alphaStep;
        }

        uint16_t base = 0;
        idx = 0;

        for (i = 0; i < MAX_STACKS; ++i)
        {
            for (j = 0; j < MAX_SLICES; ++j)
            {
                indices[idx++] = base;
                indices[idx++] = base + 1;
                indices[idx++] = base + MAX_SLICES + 1;

                indices[idx++] = base + 1;
                indices[idx++] = base + MAX_SLICES + 2;
                indices[idx++] = base + MAX_SLICES + 1;

                base++;
            }

            base++;
        }
    }

    //--------------------------------------------------------------------------

    NodePtr SGSphere::clone() const
    {
        SGSpherePtr sphere = new SGSphere();
        sphere->release();

        if (cloneProperties(sphere) != T3D_OK)
        {
            sphere = nullptr;
        }

        return sphere;
    }

    //--------------------------------------------------------------------------

    TResult SGSphere::cloneProperties(NodePtr node) const
    {
        TResult ret = SGRenderable::cloneProperties(node);

        if (ret == T3D_OK)
        {
            SGSpherePtr sphere = smart_pointer_cast<SGSphere>(node);
            ret = sphere->init(mCenter, mRadius);
        }

        return ret;
    }

    //--------------------------------------------------------------------------

    void SGSphere::updateTransform()
    {
        if (isDirty())
        {
            mBound->updateBound(getLocalToWorldTransform());
        }

        SGRenderable::updateTransform();
    }

    //--------------------------------------------------------------------------

    void SGSphere::frustumCulling(BoundPtr bound, RenderQueuePtr queue)
    {
        if (bound->test(mBound))
        {
            // 在视锥体内，放进去渲染队列，准备渲染
            queue->addRenderable(RenderQueue::E_GRPID_WIREFRAME, this);
        }
    }

    //--------------------------------------------------------------------------

    MaterialPtr SGSphere::getMaterial() const
    {
        return nullptr;
    }

    //--------------------------------------------------------------------------

    VertexArrayObjectPtr SGSphere::getVertexArrayObject() const
    {
        return mVAO;
    }
}
