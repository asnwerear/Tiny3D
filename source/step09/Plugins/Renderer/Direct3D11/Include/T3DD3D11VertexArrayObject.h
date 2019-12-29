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


#ifndef __T3D_D3D11_VERTEX_ARRAY_OBJECT_H__
#define __T3D_D3D11_VERTEX_ARRAY_OBJECT_H__


#include "T3DD3D11Prerequisites.h"


namespace Tiny3D
{
    /**
     * @brief Direct3D9 渲染器相关的VAO类
     */
    class D3D11VertexArrayObject : public VertexArrayObject
    {
    public:
        /**
         * @brief 创建 Direct3D9 渲染器相关的 VAO 对象
         */
        static D3D11VertexArrayObjectPtr create(bool useIndices);

        /**
         * @brief 析构函数
         */
        virtual ~D3D11VertexArrayObject();

        /**
         * @brief 开始绑定VAO，实现基类接口
         */
        virtual TResult beginBinding() override;

        /**
         * @brief 结束绑定VAO，实现基类接口
         */
        virtual TResult endBinding() override;

        /**
         * @brief 设置绘制图元类型，实现基类接口
         */
        virtual TResult setPrimitiveType(
            Renderer::PrimitiveType priType) override;

        /**
         * @brief 获取渲染图元类型，实现基类接口
         */
        virtual Renderer::PrimitiveType getPrimitiveType() const override;

        /**
         * @brief 设置顶点声明，实现基类接口
         */
        virtual TResult setVertexDeclaration(VertexDeclarationPtr decl) override;

        /**
         * @brief 获取顶点声明，实现基类接口
         */
        virtual VertexDeclarationPtr getVertexDeclaration() const override;

        /**
         * @brief 添加顶点缓冲区对象，实现基类接口
         */
        virtual TResult addVertexBuffer(HardwareVertexBufferPtr vbo) override;

        /**
         * @brief 获取顶点缓冲区对象数量，实现基类接口
         */
        virtual size_t getVertexBufferCount() const override;

        /**
         * @brief 获取顶点缓冲区对象，实现基类接口
         */
        virtual HardwareVertexBufferPtr getVertexBuffer(size_t idx) const override;

        /**
         * @brief 设置索引缓冲对象，实现基类接口
         */
        virtual TResult setIndexBuffer(HardwareIndexBufferPtr ibo) override;

        /**
         * @brief 获取索引缓冲对象，实现基类接口
         */
        virtual HardwareIndexBufferPtr getIndexBuffer() const override;

        /**
         * @brief 获取是否使用索引缓冲区，实现基类接口
         */
        virtual bool isIndicesUsed() const override;

        /**
         * @fn  ID3D11Buffer * const 
         *      *D3D11VertexArrayObject::getD3D11BufferArray()
         * @brief   获取 ID3D11Buffer 对象数组
         * @returns 返回 ID3D11Buffer 对象数组.
         */
        ID3D11Buffer * const *getD3D11Buffers() { return mD3D11Buffers; }

        /**
         * @fn  UINT *D3D11VertexArrayObject::getD3D11BufferStride()
         * @brief   获取 ID3D11Buffer 每个缓冲区的顶点步长
         * @returns 返回 ID3D11Buffer 每个缓冲区的顶点步长.
         */
        UINT *getD3D11BufferStrides() { return mD3D11BufferStrides; }

        /**
         * @fn  UINT D3D11VertexArrayObject::*getD3D11BufferOffsets()
         * @brief   获取 ID3D11Buffer 每个缓冲区的偏移
         * @returns 返回 ID3D11Buffer 每个缓冲区的偏移.
         */
        UINT *getD3D11BufferOffsets() { return mD3D11BufferOffsets; }

        /**
         * @fn  size_t D3D11VertexArrayObject::getVertexCount() const;
         * @brief   获取顶点数量
         * @returns 返回顶点数量.
         */
        size_t getVertexCount() const;

    protected:
        /**
         * @brief 构造函数
         */
        D3D11VertexArrayObject(bool useIndices);

    protected:
        typedef TList<HardwareVertexBufferPtr>      VBOList;
        typedef VBOList::iterator                   VBOListItr;
        typedef VBOList::const_iterator             VBOListConstItr;

        Renderer::PrimitiveType mPrimitiveType; /**< 渲染图元 */

        VertexDeclarationPtr    mDecl;          /**< 顶点声明对象 */
        VBOList                 mVBOList;       /**< 顶点缓冲区对象集合 */
        HardwareIndexBufferPtr  mIBO;           /**< *索引缓冲区对象 */

        bool            mUseIndices;            /**< 是否使用索引缓冲 */

        ID3D11Buffer    **mD3D11Buffers;        /**< D3D11Buffer 数组 */
        UINT            *mD3D11BufferStrides;   /**< 每个缓冲区的顶点步长 */
        UINT            *mD3D11BufferOffsets;   /**< 每个缓冲区的偏移 */
    };
}


#endif  /*__T3D_D3D11_VERTEX_ARRAY_OBJECT_H__*/

