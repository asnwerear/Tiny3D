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


namespace Tiny3D
{
    template <typename T>
    inline TIntrSphereTriangle<T>::TIntrSphereTriangle()
        : mSphere(nullptr)
        , mTriangle(nullptr)
    {

    }

    template <typename T>
    inline TIntrSphereTriangle<T>::TIntrSphereTriangle(
        const TSphere<T> &sphere, 
        const TTriangle<T> &triangle)
        : mSphere(&sphere)
        , mTriangle(&triangle)
    {

    }

    template <typename T>
    inline TIntrSphereTriangle<T>::TIntrSphereTriangle(
        const TSphere<T> *sphere,
        const TTriangle<T> *triangle)
        : mSphere(sphere)
        , mTriangle(triangle)
    {

    }

    template <typename T>
    bool TIntrSphereTriangle<T>::test()
    {
        if (mSphere == nullptr || mTriangle == nullptr)
            return false;


        return true;
    }
}
