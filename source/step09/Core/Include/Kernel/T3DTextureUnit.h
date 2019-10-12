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


#ifndef __T3D_TEXTURE_UNIT_H__
#define __T3D_TEXTURE_UNIT_H__


#include "T3DPrerequisites.h"
#include "T3DTypedef.h"
#include "T3DObject.h"
#include "Resource/T3DTexture.h"
#include "Kernel/T3DCommon.h"
#include "Kernel/T3DBlendMode.h"


namespace Tiny3D
{
    

    class T3D_ENGINE_API TextureUnit : public Object
    {
    public:
        enum TextureAddressMode
        {
            E_TAM_UNKNOWN = 0,
            E_TAM_WRAP,
            E_TAM_MIRROR,
            E_TAM_CLAMP,
            E_TAM_BORDER,
        };

        /**
         * @brief 描述纹理绑定到渲染流水线中可编程流水线阶段上
         */
        enum BindingType
        {
            E_BT_FRAGMENT = 0,
            E_BT_VERTEX,
            E_BT_GEOMETRY,
            E_BT_TESSELATION_HULL,
            E_BT_TESSELATION_DOMAIN,
            E_BT_COMPUTE
        };

        /**
         * @brief 描述纹理内容的标识
         */
        enum ContentType
        {
            E_CONTENT_NAMED = 0,    /// 通过名字标识纹理
            E_CONTENT_SHADOW,       /// 阴影纹理
        };

        /**
         * @brief 纹理效果
         * @remakrs 可编程渲染管线下无效，可以使用 vertex/fragment shader 实现
         */
        enum EffectType
        {
            E_ET_ENVIRONMENT_MAP = 0,   /// 环境映射
            E_ET_PROJECTIVE_TEXTURE,    /// 投影纹理映射
            E_ET_UVSCROLL,
            E_ET_USCROLL,
            E_ET_VSCROLL,
            E_ET_ROTATE,
            E_ET_TRANSFORM
        };

        enum EnvMapType
        {
            E_ENV_PLANAR = 0,
            E_ENV_CURVED,
            E_ENV_REFLECTION,
            E_ENV_NORMAL
        };

        enum TransformType
        {
            E_TT_TRANSLATE_U = 0,
            E_TT_TRANSLATE_V,
            E_TT_SCALE_U,
            E_TT_SCALE_V,
            E_TT_ROTATE
        };

        enum TextureCubeFace
        {
            E_TCF_FRONT = 0,
            E_TCF_BACK,
            E_TCF_LEFT,
            E_TCF_RIGHT,
            E_TCF_UP,
            E_TCF_DOWN
        };

        struct UVWAddressMode
        {
            TextureAddressMode u, v, w;
        };

        struct TextureEffect
        {
            EffectType      type;
            int32_t         subtype;
            Real            arg1, arg2;
            WaveformType    waveType;
            Real            base;
            Real            frequency;
            Real            phase;
            Real            amplitude;
        };

    public:

        TextureUnit();

        virtual ~TextureUnit();

    protected:
        typedef TMultimap<EffectType, TextureEffect>    EffectMap;
        typedef EffectMap::iterator                     EffectMapItr;
        typedef EffectMap::const_iterator               EffectMapConstItr;
        typedef EffectMap::value_type                   EffectMapValue;

        Pass        *mParent;           /// 当前纹理单元所属的pass对象

        String      mName;              /// 纹理单元名称

        //---------------------------------------
        // Command : texture_alias
        // Usage : texture_alias <name>
        String      mNameAlias;         /// 纹理单元别名

        //---------------------------------------
        // Command : texture
        // Usage : texture <texturename> [<type>] [umlimited|numMipmaps] [alpha] [<PixelFormat>] [gamma]
        // Parameters : 
        //  #1 texturename 存放在 mFrames 中
        TextureType mTextureType;       /// 纹理类型
        PixelFormat mTextureFormat;     /// 纹理格式
        int32_t     mSrcMipmaps;        /// 原纹理的mipmaps数量
        bool        mIsAlpha;           /// 纹理是否用于亮度的
        bool        mHWGamma;           /// 是否开启硬件gamma校正
        bool        mCubic;             /// 是否cubemap

        //---------------------------------------
        // Command : anim_texture
        // Usage : 
        //  #1 anim_texture <base_name> <num_frames> <duration>
        //  #2 anim_texture <frame1> <frame2> ... <duration>
        uint16_t    mCurrentFrame;      /// 当前帧计数
        Real        mAnimDuration;      /// 动画持续时长
        StringArray mFrames;

        //---------------------------------------
        // Command : cubic_texture
        // Usage : 
        //  #1 cubic_texture <base_name> <combineUVW|separateUV>
        //  #2 cubic_texture <front> <back> <left> <right> <up><down> separateUV
        // Parameters : 
        //  #1 所有的都跟 anim_texture 共用
        //  #2 combineUVW 设置 mTextureType 为 E_TEX_TYPE_CUBE_MAP
        //  #3 separateUV 设置 mTextureType 为 E_TEX_TYPE_2D
        
        //---------------------------------------
        // Command : binding_type
        // Usage : binding_type <vertex|fragment>
        BindingType     mBindingType;   /// 纹理绑定到渲染流水线可编程阶段

        //---------------------------------------
        // Command : content_type
        // Usage : content_type <named|shadow>
        ContentType     mContentType;   /// 纹理标识方式，普通加载的还是自动创建的

        //---------------------------------------
        // Command : tex_coord_set
        // Usage : tex_coord_set <set_num>
        uint32_t    mTexCoordSetIndex;  /// 当前材质使用的纹理层级索引

        //---------------------------------------
        // Command : tex_address_mode
        // Usage : 
        //  #1 tex_address_mode <uvw_mode>
        //  #2 tex_address_mode <u_mode> <v_mode> [<w_mode>]
        UVWAddressMode  mAddressMode;   /// 3个维度的纹理寻址模式

        //---------------------------------------
        // Command : tex_border_color
        // Usage : tex_border_color <red> <green> <blue> [<alpha>]
        ColorARGB   mBorderColor;       /// border 模式下的边缘颜色

        //---------------------------------------
        // Command : filtering
        // Usage : 
        //  #1 filtering <none|bilinear|trilinear|anisotropic>
        //  #2 filtering <minification> <magnification> <mip>
        FilterOptions   mMinFilter;
        FilterOptions   mMagFilter;
        FilterOptions   mMipFilter;
        bool            mIsDefaultFiltering;

        //---------------------------------------
        // Command : compare_test
        // Usage : compare_test <on|off>
        bool            mCompareEnabled;

        //---------------------------------------
        // Command : compare_func
        // Usage : compare_func <always_fail|always_pass|less|less_equal|equal|not_equal|greater_equal|greater>
        CompareFunction mCompareFunc;

        //---------------------------------------
        // Command : max_anisotropy
        // Usage : max_anisotropy <value>
        uint32_t        mMaxAnisotropy;
        bool            mIsDefaultAniso;

        //---------------------------------------
        // Command : mipmap_bias
        // Usage : mipmap_bias <value>
        Real        mMipmapBias;

        //---------------------------------------
        // Command : color_op
        // Usage : color_op <replace|add|modulate|alpha_blend>
        // Parameters :
        //  #1 replace 相当于 color_op_ex source1 src_texture src_current
        //  #2 add 相当于 color_op_ex add src_texture src_current
        //  #3 modulate 相当于 color_op_ex modulate src_textuer src_current
        //  #4 alpha_blend 相当于 color_op_ex blend_texture_alpha src_texture src_current

        //---------------------------------------
        // Command : color_op_ex
        // Usage : color_op_ex <operation> <source1> <source2> [<manual_factor>] [<manual_color1>] [<manual_color2>]
        LayerBlendModeEx    mColorBlendMode;

        //---------------------------------------
        // Command : color_op_multipass_fallback
        // Usage : color_op_multipass_fallback <src_factor> <dest_factor>
        BlendFactor         mColorBlendFallbackSrc;
        BlendFactor         mColorBlendFallbackDst;

        //---------------------------------------
        // Command : env_map
        // Usage : env_map <off|spherical|planar|cubic_reflection|cubic_normal>
        EffectMap           mEffects;

        //---------------------------------------
        // Command : scroll
        // Usage : scroll <x> <y>
        Real                mUOffset;
        Real                mVOffset;

        //---------------------------------------
        // Command : scroll_anim
        // Usage : scroll_anim <xspeed> <yspeed>
        // Parameters : 存储在 mEffects 里

        //---------------------------------------
        // Command : rotate
        // Usage : rotate <angle>
        Radian              mRotate;

        //---------------------------------------
        // Command : rotate_anim
        // Usage : rotate_anim <revs_per_second>
        // Parameters : 存储在 mEffects 里

        //---------------------------------------
        // Command : scale
        // Usage : scale <x_scale> <y_scale>
        Real                mUScale;
        Real                mVScale;

        //---------------------------------------
        // Command : wave_xform
        // Usage : wave_xform <xform_type> <wave_type> <base> <frequency> <phase> <amplitude>
        // Parameters : 存储在 mEffects 里

        //---------------------------------------
        // Command transform
        // Usage : transform m00 m01 m02 m03 m10 m11 m12 m13 m20 m21 m22 m23 m30 m31 m32 m33
        Matrix4             mTexMatrix;
    };
}


#endif  /*__T3D_TEXTURE_UNIT_H__*/