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


#ifndef __T3D_TEXTURE_MANAGER_H__
#define __T3D_TEXTURE_MANAGER_H__


#include "Resource/T3DResourceManager.h"
#include "Resource/T3DTexture.h"
#include "Kernel/T3DCommon.h"


namespace Tiny3D
{
    /**
     * @brief 纹理资源管理器
     */
    class T3D_ENGINE_API TextureManager
        : public Singleton<TextureManager>
        , public ResourceManager
    {
    public:
        /** 
         * @brief 创建纹理管理器对象
         */
        static TextureManagerPtr create();

        /** 
         * @brief 析构函数 
         */
        virtual ~TextureManager();

        /**
         * @brief 加载纹理资源
         * @param [in] name : 纹理资源名称
         * @param [in] mipmaps : 纹理层级数量，默认是图片中的数量
         * @param [in] texUsage : 纹理用途，默认是从图片加载
         * @param [in] texType : 纹理类型，默认是2D纹理
         * @return 返回新建的纹理对象
         */
        virtual TexturePtr loadTexture(const String &name, 
            size_t mipmaps = -1, 
            Texture::TexUsage texUsage = Texture::E_TU_DEFAULT, 
            TextureType texType = E_TEX_TYPE_2D);

        /**
         * @brief 加载纹理资源
         * @param [in] name : 纹理资源名称
         * @param [in] width : 纹理宽度
         * @param [in] height : 纹理高度
         * @param [in] mipmaps : 纹理层级数量，默认是图片中的数量
         * @param [in] format : 纹理像素格式，默认A8R8G8B8
         * @param [in] texUsage : 纹理用途，默认是从图片加载
         * @param [in] texType : 纹理类型，默认是2D纹理
         * @return 返回新建的纹理对象
         */
        virtual TexturePtr loadTexture(const String &name, size_t width, 
            size_t height, size_t mipmaps = -1, 
            PixelFormat format = PixelFormat::E_PF_A8R8G8B8, 
            Texture::TexUsage texUsage = Texture::E_TU_BLANK, 
            TextureType texType = E_TEX_TYPE_2D);

        /**
         * @brief 卸载纹理资源
         * @param [in] texture : 要卸载的纹理资源对象
         * @return void
         */
        virtual TResult unloadTexture(TexturePtr texture);

    protected:
        /**
         * @brief 构造函数
         */
        TextureManager();

        /**
         * @brief 重写基类接口，实现创建纹理对象
         * @see ResourcePtr Resource::create(const String &name, int32_t argc,
         *      va_list args)
         */
        virtual ResourcePtr create(const String &name, int32_t argc,
            va_list args) override;

    protected:
        size_t  mDefaultMipMaps;    /**< 默认纹理层级 */
    };

    #define T3D_TEXTURE_MGR         (TextureManager::getInstance())
}


#endif  /*__T3D_TEXTURE_MANAGER_H__*/
