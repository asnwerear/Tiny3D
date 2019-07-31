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


#include "T3DR3DHardwareConstantBuffer.h"


namespace Tiny3D
{
    //--------------------------------------------------------------------------

    R3DHardwareConstantBufferPtr R3DHardwareConstantBuffer::create(
        size_t bufSize, const void *buffer, Usage usage, bool useSystemMemory, 
        bool useShadowBuffer)
    {
        R3DHardwareConstantBufferPtr ibo = new R3DHardwareConstantBuffer(
            bufSize, buffer, usage, useSystemMemory, useShadowBuffer);
        ibo->release();

        return ibo;
    }

    //--------------------------------------------------------------------------

    R3DHardwareConstantBuffer::R3DHardwareConstantBuffer(
        size_t bufSize, const void *buffer, HardwareBuffer::Usage usage, 
        bool useSystemMemory, bool useShadowBuffer)
        : HardwareConstantBuffer(bufSize, usage, useSystemMemory,
            useShadowBuffer)
    {
        mBuffer = new uint8_t[mBufferSize];
        memcpy(mBuffer, buffer, mBufferSize);
    }

    //--------------------------------------------------------------------------

    R3DHardwareConstantBuffer::~R3DHardwareConstantBuffer()
    {
        T3D_SAFE_DELETE_ARRAY(mBuffer);
    }

    //--------------------------------------------------------------------------

    size_t R3DHardwareConstantBuffer::readData(size_t offset, size_t size, 
        void *dst)
    {
        size_t bytesOfRead = 0;

        do 
        {
            void *src = lock(offset, size, HardwareBuffer::E_HBL_READ_ONLY);
            if (src == nullptr)
            {
                break;
            }

            memcpy(dst, src, size);
            bytesOfRead = size;

            unlock();
        } while (0);

        return bytesOfRead;
    }

    //--------------------------------------------------------------------------

    size_t R3DHardwareConstantBuffer::writeData(size_t offset, size_t size,
        const void *src, bool discardWholeBuffer /* = false */)
    {
        size_t bytesOfWritten = 0;

        do 
        {
            void *dst = lock(offset, size, discardWholeBuffer 
                ? HardwareBuffer::E_HBL_DISCARD : HardwareBuffer::E_HBL_NORMAL);
            if (dst == nullptr)
            {
                break;
            }

            memcpy(dst, src, size);
            bytesOfWritten = size;

            unlock();
        } while (0);

        return bytesOfWritten;
    }

    //--------------------------------------------------------------------------

    void *R3DHardwareConstantBuffer::lockImpl(size_t offset, size_t size,
        LockOptions options)
    {
        char *pLockedData = nullptr;

        do 
        {
            
        } while (0);

        return pLockedData;
    }

    //--------------------------------------------------------------------------

    TResult R3DHardwareConstantBuffer::unlockImpl()
    {
        TResult ret = T3D_OK;

        do 
        {
            
        } while (0);

        return ret;
    }
}
