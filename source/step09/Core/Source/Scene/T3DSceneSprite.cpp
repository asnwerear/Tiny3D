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


#include "Scene/T3DSceneNode.h"


namespace Tiny3D
{
    class T3D_ENGINE_API SceneCamera : public SceneNode
    {
    public:
        virtual ~SceneCamera();

        /**
         * @brief 专门提供给挂相机结点的3D变换结点使用，用于构建UVN相机.
         * @param [in] pos : 相机位置
         * @param [in] obj : 相机观察物体位置
         * @param [in] up : 上方向向量
         * @return void
         * @note UVN相机通过此接口可以构造一个变换矩阵出来
         */
        void lookAt(const Vector3 &pos, const Vector3 &obj, const Vector3 &up);
    };
}