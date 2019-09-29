﻿/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2019  Answer Wong
 * For latest info, see https://github.com/answerear/Tiny3D
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


#include "Render/T3DRenderQueue.h"
#include "Render/T3DVertexArrayObject.h"
#include "Render/T3DHardwareVertexBuffer.h"
#include "Render/T3DHardwareIndexBuffer.h"
#include "SceneGraph/T3DSGRenderable.h"
#include "SceneGraph/T3DSGCamera.h"


namespace Tiny3D
{
    //--------------------------------------------------------------------------

    RenderGroupPtr RenderGroup::create()
    {
        RenderGroupPtr group = new RenderGroup();
        group->release();
        return group;
    }

    //--------------------------------------------------------------------------

    RenderGroup::RenderGroup()
    {

    }

    //--------------------------------------------------------------------------

    RenderGroup::~RenderGroup()
    {

    }

    //--------------------------------------------------------------------------

    TResult RenderGroup::addRenderable(SGRenderablePtr renderable)
    {
        MaterialPtr material = renderable->getMaterial();

        auto itr = mRenderables.find(material);

        if (itr != mRenderables.end())
        {
            // 有相同的材质，放到相同材质渲染对象列表
            RenderableList &renderables = itr->second;
            renderables.push_back(renderable);
        }
        else
        {
            // 没有相同材质，新增一个材质渲染对象列表
            RenderableList renderables(1, renderable);
            mRenderables.insert(RenderablesValue(material, renderables));
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    void RenderGroup::clear()
    {
        mRenderables.clear();
    }

    //--------------------------------------------------------------------------

    TResult RenderGroup::render(ID groupID, RendererPtr renderer)
    {
        PolygonMode renderMode;

        ViewportPtr vp = renderer->getViewport();
        if (vp == nullptr)
        {
            return T3D_OK;
        }

        SGCameraPtr camera = vp->getCamera();

        if (RenderQueue::E_GRPID_INDICATOR == groupID
            || RenderQueue::E_GRPID_WIREFRAME == groupID)
        {
            // 指示器或者线框渲染，渲染模式需要改成线框模式
            renderMode = renderer->getPolygonMode();
            renderer->setPolygonMode(E_PM_WIREFRAME);
        }

        if (RenderQueue::E_GRPID_LIGHT != groupID)
        {
            // 不是灯光分组
            RenderablesItr itr = mRenderables.begin();

            while (itr != mRenderables.end())
            {
                MaterialPtr material = itr->first;
                renderer->setMaterial(material);

                RenderableList &renderables = itr->second;

                RenderableListItr i = renderables.begin();

                while (i != renderables.end())
                {
                    SGRenderablePtr renderable = *i;

                    // 设置渲染物体的世界变换
                    const Transform &xform 
                        = renderable->getLocalToWorldTransform();
                    const Matrix4 &m = xform.getAffineMatrix();
                    renderer->setWorldTransform(m);

                    // 根据VAO数据渲染
                    renderer->drawVertexArray(
                        renderable->getVertexArrayObject());

                    ++i;
                }

                ++itr;
            }
        }

        if (RenderQueue::E_GRPID_INDICATOR == groupID
            || RenderQueue::E_GRPID_WIREFRAME == groupID)
        {
            // 恢复渲染模式
            renderer->setPolygonMode(renderMode);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    size_t RenderGroup::calcPrimitiveCount(VertexArrayObjectPtr vao) const
    {
        Renderer::PrimitiveType priType = vao->getPrimitiveType();
        bool useIndex = vao->isIndicesUsed();
        size_t indexCount 
            = (useIndex ? vao->getIndexBuffer()->getIndexCount() : 0);
        size_t vertexCount = vao->getVertexBuffer(0)->getVertexCount();

        size_t primCount = 0;

        switch (priType)
        {
        case Renderer::E_PT_POINT_LIST:
            primCount = (useIndex ? indexCount : vertexCount);
            break;

        case Renderer::E_PT_LINE_LIST:
            primCount = (useIndex ? indexCount : vertexCount) / 2;
            break;

        case Renderer::E_PT_LINE_STRIP:
            primCount = (useIndex ? indexCount : vertexCount) - 1;
            break;

        case Renderer::E_PT_TRIANGLE_LIST:
            primCount = (useIndex ? indexCount : vertexCount) / 3;
            break;

        case Renderer::E_PT_TRIANGLE_STRIP:
            primCount = (useIndex ? indexCount : vertexCount) - 2;
            break;

        case Renderer::E_PT_TRIANGLE_FAN:
            primCount = (useIndex ? indexCount : vertexCount) - 2;
            break;
        }

        return primCount;
    }

    //--------------------------------------------------------------------------

    RenderQueuePtr RenderQueue::create()
    {
        RenderQueuePtr rq = new RenderQueue();
        rq->release();
        return rq;
    }

    //--------------------------------------------------------------------------

    RenderQueue::RenderQueue()
    {

    }

    //--------------------------------------------------------------------------

    RenderQueue::~RenderQueue()
    {

    }

    //--------------------------------------------------------------------------

    TResult RenderQueue::addRenderable(GroupID groupID, 
        SGRenderablePtr renderable)
    {
        RenderableGroupItr itr = mGroups.find(groupID);

        if (itr != mGroups.end())
        {
            /// 已经有这组渲染对象
            RenderGroupPtr &group = itr->second;
            group->addRenderable(renderable);
        }
        else
        {
            /// 不存在这组渲染对象
            RenderGroupPtr group = RenderGroup::create();
            group->addRenderable(renderable);
            mGroups.insert(RenderableGroupValue(groupID, group));
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    void RenderQueue::clear()
    {
        mGroups.clear();
    }

    //--------------------------------------------------------------------------

    TResult RenderQueue::render(RendererPtr renderer)
    {
        RenderableGroupItr itr = mGroups.begin();

        while (itr != mGroups.end())
        {
            itr->second->render(itr->first, renderer);
            ++itr;
        }

        return T3D_OK;
    }
}
