/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2017  Answer Wong
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


#ifndef __T3D_GL_APPLICATION_H__
#define __T3D_GL_APPLICATION_H__


#include "T3DPlatformPrerequisites.h"
#include "Adapter/T3DApplicationInterface.h"

namespace Tiny3D
{
    class T3D_PLATFORM_API GLApplication : public IApplication
    {
    public:
        GLApplication();
        virtual ~GLApplication();

        /**
         * @brief 初始化应用程序
         * @return 调用成功返回true，否则返回false
         */
        virtual bool init() override;

        /**
         * @brief 轮询处理应用程序事件
         */
        virtual void pollEvents() override;

        /**
         * @brief 释放应用程序对象资源
         */
        virtual void release() override;

        /**
         * @brief 获取平台原生应用对象
         */
        virtual void *getNativeAppObject() override;
    };
}



#endif  /*__T3D_APPLICATION_H__*/
