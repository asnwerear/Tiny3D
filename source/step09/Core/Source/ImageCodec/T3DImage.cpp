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


#include "ImageCodec/T3DImage.h"
#include "ImageCodec/T3DImageCodec.h"


namespace Tiny3D
{
    const uint32_t Image::FILETYPE_RAW = ImageCodecBase::E_FT_RAW;
    const uint32_t Image::FILETYPE_BMP = ImageCodecBase::E_FT_BMP;
    const uint32_t Image::FILETYPE_PNG = ImageCodecBase::E_FT_PNG;
    const uint32_t Image::FILETYPE_TGA = ImageCodecBase::E_FT_TARGA;
    const uint32_t Image::FILETYPE_JPG = ImageCodecBase::E_FT_JPEG;
    const uint32_t Image::FILETYPE_DDS = ImageCodecBase::E_FT_DDS;
    const uint32_t Image::FILETYPE_PVRTC = ImageCodecBase::E_FT_PVRTC;
    const uint32_t Image::FILETYPE_ASTC = ImageCodecBase::E_FT_ASTC;
    const uint32_t Image::FILETYPE_ETC1 = ImageCodecBase::E_FT_ETC1;
    const uint32_t Image::FILETYPE_ETC2 = ImageCodecBase::E_FT_ETC2;

    //--------------------------------------------------------------------------

    int32_t Image::calcPitch(int32_t width, int32_t bpp)
    {
        return (width * (bpp / 8) + 3) & ~3;
    }

    int32_t Image::getBPP(PixelFormat format)
    {
        int32_t bpp = 0;

        switch (format)
        {
        case E_PF_PALETTE8:
            bpp = 8;
            break;
        case E_PF_R5G6B5:
        case E_PF_A1R5G5B5:
        case E_PF_A4R4G4B4:
            bpp = 16;
            break;
        case E_PF_R8G8B8:
        case E_PF_B8G8R8:
            bpp = 24;
            break;
        case E_PF_A8R8G8B8:
        case E_PF_B8G8R8A8:
        case E_PF_X8R8G8B8:
        case E_PF_B8G8R8X8:
            bpp = 32;
            break;
        default:
            bpp = 0;
            break;
        }

        return bpp;
    }

    //--------------------------------------------------------------------------

    Image::Image()
        : mWidth(0)
        , mHeight(0)
        , mBPP(0)
        , mPitch(0)
        , mFormat(E_PF_A8R8G8B8)
        , mHasAlpha(false)
        , mIsPreMulti(false)
        , mIsDataExternal(false)
        , mIsEmpty(true)
        , mData(nullptr)
        , mDataSize(0)
    {

    }

    Image::Image(int32_t width, int32_t height, int32_t bpp, PixelFormat format)
        : mSourceType(0)
        , mWidth(width)
        , mHeight(height)
        , mBPP(bpp)
        , mPitch(0)
        , mFormat(format)
        , mHasAlpha(false)
        , mIsPreMulti(false)
        , mIsDataExternal(false)
        , mIsEmpty(false)
        , mData(nullptr)
        , mDataSize(0)
    {
        mPitch = calcPitch();
        mDataSize = mHeight * mPitch;
        mData = new uint8_t[mDataSize];
        memset(mData, 0, mDataSize);
    }

    Image::~Image()
    {
        if (!mIsDataExternal)
        {
            T3D_SAFE_DELETE_ARRAY(mData);
        }
    }

    //--------------------------------------------------------------------------

    TResult Image::destroy()
    {
        if (!mIsEmpty)
        {
            if (!mIsDataExternal)
            {
                T3D_SAFE_DELETE_ARRAY(mData);
            }

            mIsEmpty = true;
        }

        mWidth = mHeight = mBPP = mPitch = 0;

        return T3D_OK;
    }

    TResult Image::load(const String &path)
    {
        TResult ret = T3D_OK;

        do
        {
            ret = destroy();
            if (ret != T3D_OK)
            {
                break;
            }

            ret = T3D_IMAGE_CODEC.decode(path, *this);
            if (ret != T3D_OK)
            {
                break;
            }

            mIsEmpty = false;
            mIsDataExternal = false;
        }while (0);

        return ret;
    }

    TResult Image::load(DataStream &stream)
    {
        TResult ret = T3D_OK;
        
        do 
        {
            ret = destroy();
            if (ret != T3D_OK)
            {
                break;
            }

            ret = T3D_IMAGE_CODEC.decode(stream, *this);
            if (ret != T3D_OK)
            {
                break;
            }

            mIsEmpty = false;
            mIsDataExternal = false;
        } while (0);

        return ret;
    }

    TResult Image::load(uint8_t *data, int32_t width, int32_t height, 
        int32_t bpp, int32_t pitch, PixelFormat format, 
        bool copySource /* = false */)
    {
        TResult ret = T3D_OK;

        do 
        {
            ret = destroy();
            if (ret != T3D_OK)
            {
                break;
            }

            mWidth = width;
            mHeight = height;
            mBPP = bpp;
            mPitch = calcPitch();

            if (pitch != mPitch)
                break;

            mFormat = format;
            mDataSize = mPitch * mHeight;
            mSourceType = FILETYPE_RAW;

            if (copySource)
            {
                mData = new uint8_t[mDataSize];
                memcpy(mData, data, mDataSize);
                mIsDataExternal = false;
            }
            else
            {
                mData = data;
                mIsDataExternal = true;
            }

            mIsEmpty = false;
        } while (0);

        return ret;
    }

    TResult Image::save(const String &path, 
        uint32_t ft /* = FILETYPE_PNG */) const
    {
        ImageCodecBase::FileType eType = (ImageCodecBase::FileType)ft;
        return T3D_IMAGE_CODEC.encode(path, *this, eType);
    }

    TResult Image::save(DataStream &stream, 
        uint32_t ft /* = FILETYPE_PNG */) const
    {
        ImageCodecBase::FileType eType = (ImageCodecBase::FileType)ft;
        return T3D_IMAGE_CODEC.encode(stream, *this, eType);
    }

    TResult Image::flip()
    {
        return T3D_IMAGE_CODEC.flip(*this);
    }

    TResult Image::mirror()
    {
        return T3D_IMAGE_CODEC.mirror(*this);
    }

    TResult Image::fill(const Color4 &rkColor)
    {
        return T3D_IMAGE_CODEC.fill(*this, rkColor);
    }

    TResult Image::copy(const Image &image, 
        const Rect *srcRect /* = nullptr */, 
        const Rect *dstRect /* = nullptr */, 
        Filter filter /* = E_FILTER_BILINEAR */)
    {
        TResult ret = T3D_OK;

        if (isEmpty())
        {
            // 空图像，给空图像创建并且复制数据
            mData = new uint8_t[image.mDataSize];
            mDataSize = image.mDataSize;
            memcpy(mData, image.mData, image.mDataSize);
            mWidth = image.mWidth;
            mHeight = image.mHeight;
            mBPP = image.mBPP;
            mFormat = image.mFormat;
            mPitch = image.mPitch;
            mHasAlpha = image.mHasAlpha;
            mIsPreMulti = image.mIsPreMulti;
            mIsDataExternal = false;
            mIsEmpty = false;
        }
        else
        {
            ret = T3D_IMAGE_CODEC.copy(image, srcRect, *this, dstRect, filter);
        }

        return ret;
    }

    TResult Image::convert(PixelFormat format)
    {
        TResult ret = T3D_OK;

        do
        {
            if (!isEmpty())
            {

            }
        } while (0);

        return ret;
    }

    TResult Image::convert(Image &image, PixelFormat format) const
    {
        TResult ret = T3D_OK;

        do
        {
        } while (0);

        return ret;
    }

    bool Image::compare(const Image &other, 
        bool compareAlpha /* = true */) const
    {
        return false;
    }

    int32_t Image::calcPitch() const
    {
        return Image::calcPitch(mWidth, mBPP);//(mWidth * (mBPP / 8) + 3) & ~3;
    }

    void Image::getColorMask(uint32_t &redMask, uint32_t &greenMask, 
        uint32_t &blueMask, uint32_t &alphaMask) const
    {
        switch (mFormat)
        {
        case E_PF_A1R5G5B5:
        {
            redMask = Color4::RGB555_RED_MASK;
            greenMask = Color4::RGB555_GREEN_MASK;
            blueMask = Color4::RGB555_BLUE_MASK;
            alphaMask = Color4::RGB555_ALPHA_MASK;
        }
        break;
        case E_PF_R5G6B5:
        {
            redMask = Color4::RGB565_RED_MASK;
            greenMask = Color4::RGB565_GREEN_MASK;
            blueMask = Color4::RGB565_BLUE_MASK;
            alphaMask = 0;
        }
        break;
        case E_PF_A4R4G4B4:
        {
            redMask = Color4::RGB444_RED_MASK;
            greenMask = Color4::RGB444_GREEN_MASK;
            blueMask = Color4::RGB444_BLUE_MASK;
            alphaMask = Color4::RGB444_ALPHA_MASK;
        }
        break;
        case E_PF_R8G8B8:
            break;
        case E_PF_B8G8R8:
            break;
        case E_PF_A8R8G8B8:
            break;
        case E_PF_B8G8R8A8:
            break;
        case E_PF_X8R8G8B8:
            break;
        case E_PF_B8G8R8X8:
            break;
        default:
            break;
        }
    }

    int32_t Image::getBytesPerPixel() const
    {
        return (mBPP >> 3);
    }
}