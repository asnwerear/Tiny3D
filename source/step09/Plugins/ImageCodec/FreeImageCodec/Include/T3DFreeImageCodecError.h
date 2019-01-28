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


#ifndef __T3D_FREEIMAGE_CODEC_ERROR_H__
#define __T3D_FREEIMAGE_CODEC_ERROR_H__


#include "T3DFreeImageCodecPrerequisites.h"


namespace Tiny3D
{
    enum FreeImageCodecError
    {
        T3D_ERR_CODEC_ENCODE_TO_MEMORY = T3D_ERR_FREEIMAGE_CODEC,   /**< 编码到内存出错 */
        T3D_ERR_CODEC_ACQUIRE_MEMORY,                               /**< 获取内存失败 */
        T3D_ERR_CODEC_DECODE_FROM_MEMORY,                           /**< 从内存加载失败 */
        T3D_ERR_CODEC_CONVERT_TO_GREY,                              /**< 转换到灰度图失败 */
        T3D_ERR_CODEC_CONVERT_TO_32BIT,                             /**< 转换到32位图失败 */
        T3D_ERR_CODEC_CONVERT_TO_24BIT,                             /**< 转换到24位图失败 */
    };
}


#endif  /*__T3D_FREEIMAGE_CODEC_ERROR_H__*/
