﻿/*******************************************************************************
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

#ifndef __T3D_MATH_LIB_H__
#define __T3D_MATH_LIB_H__


#include "T3DMathPrerequisites.h"
#include "T3DFix32.h"
#include "T3DFix64.h"
#include "T3DReal.h"
#include "T3DDegree.h"
#include "T3DRadian.h"
#include "T3DMath.h"
#include "T3DVector2.h"
#include "T3DVector3.h"
#include "T3DVector4.h"
#include "T3DMatrix2.h"
#include "T3DMatrix3.h"
#include "T3DQuaternion.h"
#include "T3DMatrix4.h"
#include "T3DRay.h"


namespace Tiny3D
{
#define __T3D_LOW_PRECISION_FLOAT__         0       // 32位单精度浮点数
#define __T3D_HIGH_PRECISION_FLOAT__        1       // 64位双精度浮点数
#define __T3D_LOW_PRECISION_FIX__           2       // 32位定点数
#define __T3D_HIGH_PRECISION_FIX__          3       // 64位定点数

#define __T3D_REAL_TYPE__   __T3D_LOW_PRECISION_FLOAT__ // 实数的精度类型

#if __T3D_REAL_TYPE__ == __T3D_LOW_PRECISION_FIX__
    typedef fix32_t     Real;
#elif __T3D_REAL_TYPE__ == __T3D_HIGH_PRECISION_FIX__
    typedef fix64_t     Real;
#elif __T3D_REAL_TYPE__ == __T3D_LOW_PRECISION_FLOAT__
    typedef float32_t   Real;
#elif __T3D_REAL_TYPE__ == __T3D_HIGH_PRECISION_FLOAT__
    typedef float64_t   Real;
#endif


typedef TDegree<Real>       Degree;
typedef TRadian<Real>       Radian;
typedef TMath<Real>         Math;
typedef TVector2<Real>      Vector2;
typedef TVector3<Real>      Vector3;
typedef TVector4<Real>      Vector4;
typedef TMatrix2<Real>      Matrix2;
typedef TMatrix3<Real>      Matrix3;
typedef TMatrix4<Real>      Matrix4;
typedef TQuaternion<Real>   Quaternion;
typedef TRay<Real>          Ray;


#define REAL_ZERO           TReal<Real>::ZERO
#define REAL_HALF           TReal<Real>::HALF
#define REAL_ONE            TReal<Real>::ONE
#define REAL_MINUS_ONE      TReal<Real>::MINUS_ONE
#define REAL_INF            TReal<Real>::INF
#define REAL_MINUS_INF      TReal<Real>::MINUS_INF
}


#endif  /*__T3D_MATH_LIB_H__*/
