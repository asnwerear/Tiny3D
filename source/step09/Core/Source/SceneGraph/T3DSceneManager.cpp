﻿/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Agent)
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


#include "SceneGraph/T3DSceneManager.h"
#include "SceneGraph/T3DSGTransform3D.h"
#include "SceneGraph/T3DSGCamera.h"
#include "SceneGraph/T3DSGLight.h"
#include "SceneGraph/T3DSGModel.h"
#include "SceneGraph/T3DSGMesh.h"
#include "SceneGraph/T3DSGAxis.h"
#include "SceneGraph/T3DSGQuad.h"
#include "SceneGraph/T3DSGBillboard.h"
#include "SceneGraph/T3DSGBox.h"
#include "SceneGraph/T3DSGSphere.h"


namespace Tiny3D
{
    T3D_INIT_SINGLETON(SceneManager);

    //--------------------------------------------------------------------------

    SceneManagerPtr SceneManager::create(SceneManagerBase *impl)
    {
        SceneManagerPtr mgr = new SceneManager(impl);
        mgr->release();
        return mgr;
    }

    //--------------------------------------------------------------------------

    SceneManager::SceneManager(SceneManagerBase *impl)
        : mImpl(impl)
    {

    }

    //--------------------------------------------------------------------------

    SceneManager::~SceneManager()
    {
        mImpl = nullptr;
    }

    //--------------------------------------------------------------------------

    SGTransform3DPtr SceneManager::createTransform3D(SGNodePtr parent, 
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createTransform3D(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGCameraPtr SceneManager::createCamera(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createCamera(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGLightPtr SceneManager::createLight(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createLight(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGModelPtr SceneManager::createModel(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createModel(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGMeshPtr SceneManager::createMesh(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createMesh(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGAxisPtr SceneManager::createAxis(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createAxis(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGQuadPtr SceneManager::createQuad(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createQuad(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGBillboardPtr SceneManager::createBillboard(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createBillboard(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGBoxPtr SceneManager::createBox(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createBox(parent, uID);
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------

    SGSpherePtr SceneManager::createSphere(SGNodePtr parent,
        ID uID /* = Node::E_NID_AUTOMATIC */)
    {
        if (mImpl != nullptr)
        {
            return mImpl->createSphere(parent, uID);
        }

        return nullptr;
    }
}
