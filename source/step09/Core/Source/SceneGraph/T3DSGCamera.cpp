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


#include "SceneGraph/T3DSGCamera.h"
#include "SceneGraph/T3DSceneManager.h"
#include "SceneGraph/T3DSGTransform3D.h"
#include "Render/T3DViewport.h"
#include "Render/T3DRenderer.h"
#include "Kernel/T3DAgent.h"
#include "Bound/T3DFrustumBound.h"


namespace Tiny3D
{
    //--------------------------------------------------------------------------

    SGCameraPtr SGCamera::create(ID uID /* = E_NID_AUTOMATIC */)
    {
        SGCameraPtr camera = new SGCamera(uID);
        camera->release();
        return camera;
    }

    //--------------------------------------------------------------------------

    SGCamera::SGCamera(ID uID /* = E_NID_AUTOMATIC */)
        : SGTransform3D(uID)
        , mBound(nullptr)
        , mViewport(nullptr)
        , mProjType(E_PT_PERSPECTIVE)
        , mObjectMask(0)
        , mLeft(REAL_ZERO)
        , mRight(REAL_ZERO)
        , mTop(REAL_ZERO)
        , mBottom(REAL_ZERO)
        , mNear(REAL_ZERO)
        , mFar(REAL_ZERO)
        , mViewMatrix(false)
        , mProjMatrix(false)
        , mIsViewDirty(false)
        , mIsFrustumDirty(false)
    {

    }

    //--------------------------------------------------------------------------

    SGCamera::~SGCamera()
    {
        mBound = nullptr;
        mViewport = nullptr;
    }

    //--------------------------------------------------------------------------

    Node::Type SGCamera::getNodeType() const
    {
        return E_NT_CAMERA;
    }

    //--------------------------------------------------------------------------

    void SGCamera::renderScene(ViewportPtr viewport)
    {
        mViewport = viewport;
        T3D_SCENE_MGR.renderScene(this);
        mViewport = nullptr;
    }

    //--------------------------------------------------------------------------

    void SGCamera::lookAt(const Vector3 &pos, const Vector3 &obj, 
        const Vector3 &up)
    {
        Vector3 N = obj - pos;
        N.normalize();
        Vector3 V = up;
        V.normalize();
        Vector3 U = N.cross(V);
        U.normalize();
        V = U.cross(N);

        setPosition(pos);

        Matrix3 mat;
        mat.setColumn(0, U);
        mat.setColumn(1, V);
        mat.setColumn(2, -N);
        Quaternion orientation;
        orientation.fromRotationMatrix(mat);
        setOrientation(orientation);

        setScaling(Vector3::UNIT_SCALE);
    }

    //--------------------------------------------------------------------------

    void SGCamera::setDirty(bool isDirty, bool recursive /* = false */)
    {
        SGTransform3D::setDirty(isDirty, recursive);
        mIsViewDirty = isDirty;
    }

    //--------------------------------------------------------------------------

    const Matrix4 &SGCamera::getViewMatrix() const
    {
        if (mIsViewDirty)
        {
            // 视图矩阵推导：
            // 其中C是相机进行世界变换的矩阵，
            //  T是平移变换
            //  R是旋转变换
            //  S是缩放变换
            //
            // 由 C = T * R * S
            // 得 C(-1) = (T * R * S) (-1) = S(-1) * R(-1) * T(-1)
            // 

            const Transform &transform = getLocalToWorldTransform();
            // 旋转矩阵
            Matrix4 R = transform.getOrientation();
            // 旋转矩阵是正交矩阵，正交矩阵的逆矩阵是其转置矩阵
            Matrix4 invertR = R.transpose();
            // 平移矩阵
            Matrix4 invertT(false);
            invertT.makeTranslate(-transform.getTranslation());
            // 缩放矩阵
            Matrix4 invertS(false);
            const Vector3 &scale = transform.getScaling();
            invertS[0][0] = REAL_ONE / scale.x();
            invertS[1][1] = REAL_ONE / scale.y();
            invertS[2][2] = REAL_ONE / scale.z();

            mViewMatrix = invertS * invertR * invertT;

            mIsViewDirty = false;
        }

        return mViewMatrix;
    }

    //--------------------------------------------------------------------------

    const Matrix4 &SGCamera::getProjectionMatrix() const
    {
        if (mIsFrustumDirty)
        {
            RendererPtr renderer = T3D_AGENT.getActiveRenderer();

            switch (mProjType)
            {
            case E_PT_ORTHOGRAPHIC:
                {
                    mProjMatrix = renderer->orthographic(mLeft, mRight,
                        mTop, mBottom, mNear, mFar);
                }
                break;
            case E_PT_PERSPECTIVE:
            default:
                {
                    mProjMatrix = renderer->perspective(mLeft, mRight,
                        mTop, mBottom, mNear, mFar);
                }
                break;
            }

            mIsFrustumDirty = false;
        }

        return mProjMatrix;
    }

    //--------------------------------------------------------------------------

    NodePtr SGCamera::clone() const
    {
        SGCameraPtr camera = create();
        
        if (cloneProperties(camera) != T3D_OK)
        {
            camera = nullptr;
        }

        return camera;
    }

    //--------------------------------------------------------------------------

    TResult SGCamera::cloneProperties(NodePtr node) const
    {
        TResult ret = SGNode::cloneProperties(node);

        if (ret == T3D_OK)
        {
            SGCameraPtr camera = smart_pointer_cast<SGCamera>(node);
            camera->mProjType = mProjType;
            camera->mObjectMask = mObjectMask;
            camera->mLeft = mLeft;
            camera->mRight = mRight;
            camera->mTop = mTop;
            camera->mBottom = mBottom;
            camera->mNear = mNear;
            camera->mFar = mFar;
            camera->mViewMatrix = mViewMatrix;
            camera->mProjMatrix = mProjMatrix;
            camera->mIsViewDirty = mIsViewDirty;
            camera->mIsFrustumDirty = mIsFrustumDirty;
        }

        return ret;
    }

    //--------------------------------------------------------------------------

    void SGCamera::updateTransform()
    {
        SGTransform3D::updateTransform();

        bool isViewDirty = mIsViewDirty;
        bool isFrustumDirty = mIsFrustumDirty;

        const Matrix4 &V = getViewMatrix();
        const Matrix4 &P = getProjectionMatrix();

        if (isViewDirty || isFrustumDirty)
        {
            // 相机比较特殊，直接先更新自身的frustum，
            // 避免其他物体无法做frustum culling
            Matrix4 M = P * V;
            Renderer *renderer = T3D_AGENT.getActiveRenderer();
            FrustumBoundPtr bound = smart_pointer_cast<FrustumBound>(mBound);
            renderer->updateFrustum(M, bound);
        }
    }
}