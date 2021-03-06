/***************************************************************************************************
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
 **************************************************************************************************/

#include "SceneGraph/T3DSGSprite.h"


namespace Tiny3D
{
    SGSpritePtr SGSprite::create(uint32_t uID /* = E_NID_AUTOMATIC */)
    {
        SGSpritePtr spr = new SGSprite(uID);
        if (spr != nullptr && spr->init())
        {
            spr->release();
        }
        else
        {
            T3D_SAFE_RELEASE(spr);
        }
        return spr;
    }

    SGSprite::SGSprite(uint32_t uID /* = E_NID_AUTOMATIC */)
        : SGRenderable(uID)
    {

    }

    SGSprite::~SGSprite()
    {

    }

    bool SGSprite::init()
    {
        return true;
    }

    Node::Type SGSprite::getNodeType() const
    {
        return E_NT_SPRITE;
    }

    NodePtr SGSprite::clone() const
    {
        return SGSprite::create();
    }

    void SGSprite::setTextureUV(size_t index, const Vector2 &uv)
    {

    }

    void SGSprite::setTextureUV(const Rect &rect)
    {

    }

    void SGSprite::setVertexColor(size_t index, const Color4 &color)
    {

    }

    void SGSprite::setVerticesColor(const Color4 &color)
    {

    }

    void SGSprite::setAnchorPos(const Vector2 &pos)
    {

    }

    const Vector2 &SGSprite::getAnchorPos() const
    {
        return mAnchorPos;
    }

    void SGSprite::setSize(const Size &size)
    {

    }

    const Size &SGSprite::getSize() const
    {
        return mSize;
    }

    void SGSprite::frustumCulling(const BoundPtr &bound, const RenderQueuePtr &queue)
    {

    }

    MaterialPtr SGSprite::getMaterial() const
    {
        return mMaterial;
    }

    Renderer::PrimitiveType SGSprite::getPrimitiveType() const
    {
        return Renderer::E_PT_TRIANGLE_LIST;
    }

    VertexDataPtr SGSprite::getVertexData() const
    {
        return mVertexData;
    }

    IndexDataPtr SGSprite::getIndexData() const
    {
        return nullptr;
    }

    bool SGSprite::isIndicesUsed() const
    {
        return false;
    }
}
