﻿/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2020  Answer Wong
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


#ifndef __T3D_COMPONENT_H__
#define __T3D_COMPONENT_H__


#include "T3DPrerequisites.h"
#include "T3DTypedef.h"
#include "Kernel/T3DObject.h"


namespace Tiny3D
{
    /**
     * @class   Component
     * @brief   组件类
     */
    class T3D_ENGINE_API Component : Object
    {
        T3D_DECLARE_CLASS();

    public:
        /**
         * @fn  Component::Component();
         * @brief   构造函数
         */
        Component();

        /**
         * @fn  virtual Component::~Component();
         * @brief   析构函数
         */
        virtual ~Component();
    };
}


#endif  /*__T3D_COMPONENT_H__*/