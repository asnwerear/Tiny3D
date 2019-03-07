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


#include "SceneGraph/T3DDefaultSceneMgr.h"
#include "Render/T3DRenderQueue.h"
#include "Render/T3DViewport.h"
#include "SceneGraph/T3DSGTransform3D.h"
#include "SceneGraph/T3DSGCamera.h"
#include "SceneGraph/T3DSGLight.h"
#include "SceneGraph/T3DSGModel.h"
#include "SceneGraph/T3DSGMesh.h"
#include "SceneGraph/T3DSGAxis.h"
#include "SceneGraph/T3DSGBillboard.h"
#include "SceneGraph/T3DSGQuad.h"
#include "SceneGraph/T3DSGBox.h"
#include "SceneGraph/T3DSGSphere.h"
#include "Kernel/T3DAgent.h"


namespace Tiny3D
{
    //--------------------------------------------------------------------------

    T3D_INIT_SINGLETON(DefaultSceneMgr);

    //--------------------------------------------------------------------------

    DefaultSceneMgrPtr DefaultSceneMgr::create()
    {
        DefaultSceneMgrPtr mgr = new DefaultSceneMgr();
        mgr->release();

        if (mgr->init() != T3D_OK)
        {
            mgr = nullptr;
        }

        return mgr;
    }

    //--------------------------------------------------------------------------

    DefaultSceneMgr::DefaultSceneMgr()
        : mRoot(nullptr)
        , mRenderQueue(nullptr)
    {

    }

    //--------------------------------------------------------------------------

    DefaultSceneMgr::~DefaultSceneMgr()
    {
        mRoot = nullptr;
        mRenderQueue = nullptr;
    }

    //--------------------------------------------------------------------------

    TResult DefaultSceneMgr::init()
    {
        TResult ret = T3D_OK;

        mRenderQueue = RenderQueue::create();

        mRoot = SGTransform3D::create();
        mRoot->setName("Root");

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult DefaultSceneMgr::update()
    {
        TResult ret = T3D_OK;

        if (mRoot != nullptr)
        {
            mRoot->visit();
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult DefaultSceneMgr::render(ViewportPtr viewport)
    {
        TResult ret = T3D_OK;

        RendererPtr renderer = T3D_AGENT.getActiveRenderer();

        // 设置当前视口
        renderer->setViewport(viewport);

        // 清空渲染队列
        mRenderQueue->clear();
        
        // 直接对渲染队列的对象渲染
        renderer->beginRender();
        mRenderQueue->render(renderer);
        renderer->endRender();

        return ret;
    }

    //--------------------------------------------------------------------------

    SGNodePtr DefaultSceneMgr::getRoot() const
    {
        return mRoot;
    }

    //--------------------------------------------------------------------------

    SGTransform3DPtr DefaultSceneMgr::createTransform3D(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGTransform3DPtr node = SGTransform3D::create(uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGCameraPtr DefaultSceneMgr::createCamera(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGCameraPtr node = SGCamera::create(uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGLightPtr DefaultSceneMgr::createLight(SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGLightPtr node = SGLight::create(uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGModelPtr DefaultSceneMgr::createModel(SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGModelPtr node = SGModel::create(uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGMeshPtr DefaultSceneMgr::createMesh(SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGMeshPtr node = SGMesh::create(uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGAxisPtr DefaultSceneMgr::createAxis(Real X, Real Y, Real Z, 
        SGNodePtr parent, ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGAxisPtr node = SGAxis::create(X, Y, Z, uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGQuadPtr DefaultSceneMgr::createQuad(const SGQuad::Quad &quad, 
        const String &materialName, SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGQuadPtr node = SGQuad::create(quad, materialName, uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGBillboardPtr DefaultSceneMgr::createBillboard(SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGBillboardPtr node = SGBillboard::create(uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGBoxPtr DefaultSceneMgr::createBox(const Vector3 &center, 
        const Vector3 &extent, SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGBoxPtr node = SGBox::create(center, extent, uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }

    //--------------------------------------------------------------------------

    SGSpherePtr DefaultSceneMgr::createSphere(const Vector3 &center, 
        Real radius, SGNodePtr parent, ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        SGSpherePtr node = SGSphere::create(center, radius, uID);

        if (parent != nullptr)
        {
            parent->addChild(node);
        }

        return node;
    }
}

