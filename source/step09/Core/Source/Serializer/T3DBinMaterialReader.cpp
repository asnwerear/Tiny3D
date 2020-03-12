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


#include "Serializer/T3DBinMaterialReader.h"
#include "T3DErrorDef.h"
#include "Kernel/T3DCommon.h"
#include "Resource/T3DMaterial.h"
#include "Resource/T3DGPUProgram.h"
#include "Resource/T3DGPUConstBuffer.h"
#include "Kernel/T3DTechnique.h"
#include "Kernel/T3DPass.h"
#include "Kernel/T3DTextureUnit.h"
#include "protobuf/MaterialScriptObject.pb.h"


namespace Tiny3D
{
    using namespace Script;

    #define T3D_CHECK_PARSING_RET(ret)    \
        if (T3D_FAILED(ret))    \
        {   \
            break;  \
        }

    //--------------------------------------------------------------------------

    T3D_IMPLEMENT_CLASS_1(BinMaterialReader, MaterialReader);

    //--------------------------------------------------------------------------

    BinMaterialReaderPtr BinMaterialReader::create()
    {
        BinMaterialReaderPtr reader = new BinMaterialReader();
        reader->release();
        return reader;
    }

    //--------------------------------------------------------------------------

    BinMaterialReader::BinMaterialReader()
    {

    }

    //--------------------------------------------------------------------------

    BinMaterialReader::~BinMaterialReader()
    {

    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parse(DataStream &stream, Material *material)
    {
        TResult ret = T3D_OK;
        uint8_t *data = nullptr;

        do 
        {
            size_t size = stream.size();
            data = new uint8_t[size];

            size = stream.read(data);

            if (size == 0)
            {
                ret = T3D_ERR_INVALID_SIZE;
                break;
            }

            ret = parse(data, size, material);
        } while (0);

        T3D_SAFE_DELETE_ARRAY(data);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parse(
        const uint8_t *data, size_t size, Material *material)
    {
        TResult ret = T3D_OK;

        do 
        {
            // 读取文件头
            T3DFileHeader *header = (T3DFileHeader *)data;

            if (stricmp(header->magic, T3D_MAGIC) != 0)
            {
                // 非法的文件类型
                ret = T3D_ERR_RES_INVALID_FILETYPE;
                T3D_LOG_ERROR(LOG_TAG_RESOURCE, "Invalid file type !");
                break;
            }

            if (header->subtype != FileSubType::E_FST_SCRIPT)
            {
                // 非法材质脚本类型
                ret = T3D_ERR_RES_INVALID_FILETYPE;
                T3D_LOG_ERROR(LOG_TAG_RESOURCE, "Invalid file subtype !");
                break;
            }

            // 从 proto buffer 解析出来 pb 对象
            data += sizeof(header);
            MaterialSystem::Material src;

            if (!src.ParseFromArray(data, size))
            {
                ret = T3D_ERR_RES_INVALID_CONTENT;
                T3D_LOG_ERROR(LOG_TAG_RESOURCE, "Invalid file content !");
                break;
            }

            ret = parseMaterial(&src, material);
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseMaterial(
        const MaterialSystem::Material *src, Material *dst)
    {
        TResult ret = T3D_OK;

        do 
        {
            // Material name
            const MaterialSystem::Header &header = src->header();
            dst->setMaterialName(header.name());

            // lod_values & lod_strategy
            // auto values = src->values();

            // receive_shadows
            // src->receive_shadows();

            // transparency_casts_shadows
            // src->transparency_casts_shadows();

            // texture_alias
            // auto alias = src->texture_alias();
            // auto aliasName = alias.alias_name();
            // auto texName = alias.texture_name();

            // Techniques
            auto techniques = src->techniques();

            for (const MaterialSystem::Technique &tech : techniques)
            {
                ret = parseTechnique(&tech, dst);
                if (T3D_FAILED(ret))
                {
                    break;
                }
            }
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTechnique(
        const MaterialSystem::Technique *src, Material *dst)
    {
        TResult ret = T3D_OK;

        do 
        {
            // Technique name
            const MaterialSystem::Header &header = src->header();
            TechniquePtr tech;
            ret = dst->addTechnique(header.name(), tech);
            if (T3D_FAILED(ret))
            {
                T3D_LOG_ERROR(LOG_TAG_RESOURCE, 
                    "Add technique for material [%s] failed !", 
                    dst->getName().c_str());
                    break;
            }

            // render_queue
            if (src->has_render_queue())
            {
                tech->setRenderQueue(src->render_queue().value());
            }

            // lod_index
            // auto index = src->lod_index();

            // scheme
            // auto scheme = src->scheme();

            // shadow_caster_material
            // auto casterMaterial = src->shadow_caster_material();

            // shadow_receiver_material
            // auto receiverMaterial = src->shadow_receiver_material();

            // gpu_vendor_rule
            // auto vendor = src->gpu_vendor_rule();
            // auto ruleType = vendor.rule();
            // auto vendorName = vendor.vendor_name();

            // gpu_device_rule
            // auto device = src->gpu_device_rule();
            // auto ruleType = device.rule();
            // auto pattern = device.device_pattern();
            // auto caseSensitive = device.case_sensitive();

            // Passes
            auto passes = src->passes();

            for (const MaterialSystem::Pass &pass : passes)
            {
                ret = parsePass(&pass, tech);
                T3D_CHECK_PARSING_RET(ret);
            }
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePass(
        const MaterialSystem::Pass *src, Technique *dst)
    {
        TResult ret = T3D_OK;

        do 
        {
            // Pass name
            const MaterialSystem::Header &header = src->header();
            PassPtr pass;
            ret = dst->addPass(header.name(), pass);
            T3D_CHECK_PARSING_RET(ret);

            // ambient
            ret = parseAmbient(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // diffuse
            ret = parseDiffuse(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // specular
            ret = parseSpecular(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // emissive
            ret = parseEmissive(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // scene_blend
            ret = parseSceneBlend(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // separate_scene_blend
            ret = parseSeparateSceneBlend(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // scene_blend_op
            ret = parseSceneBlendOp(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // separate_scene_blend_op
            ret = parseSeparateSceneBlendOp(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // depth_check
            ret = parseDepthCheck(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // depth_write
            ret = parseDepthWrite(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // depth_func
            ret = parseDepthFunc(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // depth_bias
            ret = parseDepthBias(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // iteration_depth_bias
            ret = parseIterationDepthBias(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // alpha_rejection
            ret = parseAlphaRejection(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // alpha_to_coverage
            ret = parseAlpha2Coverage(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // light_scissor
            ret = parseLightScissor(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // light_clip_planes
            ret = parseLightClipPlanes(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // illumination_stage
            ret = parseIlluminationStage(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // normailize_normals
            ret = parseNormalizeNormals(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // transparent_sorting
            ret = parseTransparentSorting(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // cull_hardware
            ret = parseCullHardware(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // cull_software
            ret = parseCullSoftware(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // lighting
            ret = parseLighting(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // shading
            ret = parseShading(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // polygon_mode
            ret = parsePolygonMode(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // polygon_mode_overrideable
            ret = parsePolygonModeOverridable(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // fog
            ret = parseFog(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // color_write
            ret = parseColorWrite(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // max_lights
            ret = parseMaxLights(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // start_light
            ret = parseStartLight(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // iteration
            ret = parseIteration(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // point_size
            ret = parsePointSize(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // point_sprites
            ret = parsePointSprites(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // point_size_attenuation
            ret = parsePointSizeAttenuation(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // point_size_min
            ret = parsePointSizeMin(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // point_size_max
            ret = parsePointSizeMax(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // gpu_program_ref
            ret = parseGPUProgramRef(src, pass);
            T3D_CHECK_PARSING_RET(ret);

            // textures
            ret = parseTextures(src, pass);
            T3D_CHECK_PARSING_RET(ret);
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseAmbient(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_ambient())
        {
            auto ambient = src->ambient();
            if (!ambient.has_color() && ambient.vertex_color())
            {
                // use vertex color for ambient
                dst->setVertexColorTracking(TrackVertexColor::AMBIENT);
            }
            else
            {
                // color value
                auto clr = ambient.color();
                dst->setAmbient(clr.r(), clr.g(), clr.b(), clr.a());
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseDiffuse(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_diffuse())
        {
            auto diffuse = src->diffuse();
            if (!diffuse.has_color() && diffuse.vertex_color())
            {
                // use vertex color for diffuse
                dst->setVertexColorTracking(TrackVertexColor::DIFFUSE);
            }
            else
            {
                // color value
                auto clr = diffuse.color();
                dst->setDiffuse(clr.r(), clr.g(), clr.b(), clr.a());
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseSpecular(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_specular())
        {
            auto specular = src->specular();
            if (!specular.has_color() && specular.vertex_color())
            {
                // use vertex color for specular
                dst->setVertexColorTracking(TrackVertexColor::SPECULAR);
            }
            else
            {
                // color value
                auto clr = specular.color();
                dst->setSpecular(clr.r(), clr.g(), clr.b(), clr.a());
            }

            dst->setShininess(src->shininess().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseEmissive(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_emissive())
        {
            auto emissive = src->emissive();
            if (!emissive.has_color() && emissive.vertex_color())
            {
                // use vertex color for emissive
                dst->setVertexColorTracking(TrackVertexColor::EMISSIVE);
            }
            else
            {
                // color value
                auto clr = emissive.color();
                dst->setEmissive(clr.r(), clr.g(), clr.b(), clr.a());
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseSceneBlend(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_scene_blend())
        {
            auto blend = src->scene_blend();
            if (blend.has_blend_factor())
            {
                // blend factor
                auto factor = blend.blend_factor();
                BlendFactor srcFactor = (BlendFactor)factor.src_factor();
                BlendFactor dstFactor = (BlendFactor)factor.dest_factor();
                dst->setSceneBlending(srcFactor, dstFactor);
            }
            else
            {
                // blend type
                BlendType blendType = (BlendType)blend.blend_type();
                dst->setSceneBlending(blendType);
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseSeparateSceneBlend(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_separate_scene_blend())
        {
            auto blend = src->separate_scene_blend();
            if (blend.has_blend_type())
            {
                // separate blend type
                auto blendType = blend.blend_type();
                BlendType colorType = (BlendType)blendType.color_blend();
                BlendType alphaType = (BlendType)blendType.alpha_blend();
                dst->setSeparateSceneBlending(colorType, alphaType);
            }
            else
            {
                // separate blend factor
                auto blendFactor = blend.blend_factor();
                BlendFactor colorSrcF = (BlendFactor)blendFactor.color_src_factor();
                BlendFactor colorDstF = (BlendFactor)blendFactor.color_dest_factor();
                BlendFactor alphaSrcF = (BlendFactor)blendFactor.alpha_src_factor();
                BlendFactor alphaDstF = (BlendFactor)blendFactor.alpha_dest_factor();
                dst->setSeparateSceneBlending(colorSrcF, colorDstF,
                    alphaSrcF, alphaDstF);
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseSceneBlendOp(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_scene_blend_op())
        {
            BlendOperation op = (BlendOperation)(src->scene_blend_op().value());
            dst->setSceneBlendingOperation(op);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseSeparateSceneBlendOp(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_separate_scene_blend_op())
        {
            auto op = src->separate_scene_blend_op();
            BlendOperation colorOp = (BlendOperation)op.color_op();
            BlendOperation alphaOp = (BlendOperation)op.alpha_op();
            dst->setSeparateSceneBlendingOperation(colorOp, alphaOp);
        }
        
        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseDepthCheck(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_depth_check())
        {
            dst->setDepthCheckEnabled(src->depth_check().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseDepthWrite(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_depth_write())
        {
            dst->setDepthWriteEnabled(src->depth_write().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseDepthFunc(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_depth_func())
        {
            CompareFunction func = (CompareFunction)(src->depth_func().value());
            dst->setDepthFunction(func);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseDepthBias(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_depth_bias())
        {
            float32_t constant = src->depth_bias().constant_bias();
            float32_t slopeScale = src->depth_bias().slopescale_bias();
            dst->setDepthBias(constant, slopeScale);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseIterationDepthBias(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_iteration_depth_bias())
        {
            dst->setDepthBiasPerIteration(src->iteration_depth_bias().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseAlphaRejection(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_alpha_rejection())
        {
            CompareFunction func = (CompareFunction)(src->alpha_rejection().function());
            uint8_t value = (uint8_t)(src->alpha_rejection().value());
            dst->setAlphaRejectFunction(func);
            dst->setAlphaRejectValue(value);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseAlpha2Coverage(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_alpha_to_coverage())
        {
            bool enabled = src->alpha_to_coverage().value();
            dst->setAlphaToCoverageEnabled(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseLightScissor(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_light_scissor())
        {
            bool enabled = src->light_scissor().value();
            dst->setLightScissoringEnabled(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseLightClipPlanes(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_light_clip_planes())
        {
            bool enabled = src->light_clip_planes().value();
            dst->setLightClipPlanesEnabled(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseIlluminationStage(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_illumination_stage())
        {
            IlluminationStage stage = (IlluminationStage)src->illumination_stage().stage();
            dst->setIlluminationStage(stage);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseNormalizeNormals(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_normailize_normals())
        {
            bool enabled = src->normailize_normals().value();
            dst->setNormalizeNormalsEnabled(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTransparentSorting(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        TResult ret = T3D_OK;

        if (src->has_transparent_sorting())
        {
            switch (src->transparent_sorting().value())
            {
            case MaterialSystem::TransparentSorting::TS_OFF:
                {
                    dst->setTransparentSortingEnabled(false);
                    dst->setTransparentSortingForced(false);
                }
                break;
            case MaterialSystem::TransparentSorting::TS_ON:
                {
                    dst->setTransparentSortingEnabled(true);
                    dst->setTransparentSortingForced(false);
                }
                break;
            case MaterialSystem::TransparentSorting::TS_FORCE:
                {
                    dst->setTransparentSortingEnabled(true);
                    dst->setTransparentSortingForced(true);
                }
                break;
            default:
                {
                    T3D_LOG_ERROR(LOG_TAG_RESOURCE, 
                        "Invalid transparent sorting value !");
                    ret = T3D_ERR_RES_INVALID_PROPERTY;
                }
                break;
            }
        }

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseCullHardware(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_cull_hardware())
        {
            CullingMode mode = (CullingMode)(src->cull_hardware().value());
            dst->setCullingMode(mode);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseCullSoftware(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_cull_software())
        {
            ManualCullingMode mode = (ManualCullingMode)(src->cull_software().value());
            dst->setManualCullingMode(mode);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseLighting(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_lighting())
        {
            bool enabled = src->lighting().value();
            dst->setLightEnabled(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseShading(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_shading())
        {
            ShadingMode mode = (ShadingMode)(src->shading().value());
            dst->setShadingMode(mode);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePolygonMode(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_polygon_mode())
        {
            PolygonMode mode = (PolygonMode)src->polygon_mode().value();
            dst->setPolygonMode(mode);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePolygonModeOverridable(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_polygon_mode_overrideable())
        {
            bool enabled = src->polygon_mode_overrideable().value();
            dst->setPolygonModeOverrideable(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseFog(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_fog())
        {
            bool overrideable = src->fog().overrideable();

            if (overrideable)
            {
                FogMode mode = (FogMode)src->fog().type();
                auto clr = src->fog().color();
                ColorRGBA color(clr.r(), clr.g(), clr.b(), clr.a());
                float32_t density = src->fog().density();
                float32_t start = src->fog().start();
                float32_t end = src->fog().end();
                dst->setFog(overrideable, mode, color, density, start, end);
            }
            else
            {
                dst->setFog(overrideable);
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseColorWrite(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_color_write())
        {
            bool enabled = src->color_write().value();
            dst->setColorWriteEnabled(enabled);
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseMaxLights(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_max_lights())
        {
            dst->setMaxLights((uint16_t)src->max_lights().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseStartLight(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_start_light())
        {
            dst->setStartLight((uint16_t)src->start_light().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseIteration(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        TResult ret = T3D_OK;

        if (src->has_iteration())
        {
            auto iter = src->iteration();

            if (iter.has_once())
            {
                // once
                dst->setIteratePerLight(false);
            }
            else if (iter.has_once_per_light())
            {
                // once per light
                auto data = iter.once_per_light();

                // light type
                switch (data.light_type())
                {
                case Script::MaterialSystem::LT_POINT:
                    dst->setIteratePerLight(true);
                    break;
                case Script::MaterialSystem::LT_DIRECTIONAL:
                    dst->setIteratePerLight(true, true, LightType::DIRECTIONAL);
                    break;
                case Script::MaterialSystem::LT_SPOT:
                    dst->setIteratePerLight(true, true, LightType::SPOTLIGHT);
                    break;
                default:
                    ret = T3D_ERR_RES_INVALID_PROPERTY;
                    T3D_LOG_ERROR(LOG_TAG_RESOURCE, "Invalid iteration data !");
                    break;
                }
            }
            else if (iter.has_per_light())
            {
                // per light
                auto data = iter.per_light();

                // number
                dst->setPassIterationCount(data.number());

                // light type
                switch (data.light_type())
                {
                case Script::MaterialSystem::LT_POINT:
                    dst->setIteratePerLight(true);
                    break;
                case Script::MaterialSystem::LT_DIRECTIONAL:
                    dst->setIteratePerLight(true, true, LightType::DIRECTIONAL);
                    break;
                case Script::MaterialSystem::LT_SPOT:
                    dst->setIteratePerLight(true, true, LightType::SPOTLIGHT);
                    break;
                default:
                    ret = T3D_ERR_RES_INVALID_PROPERTY;
                    T3D_LOG_ERROR(LOG_TAG_RESOURCE, "Invalid iteration data !");
                    break;
                }
            }
            else if (iter.has_per_n_light())
            {
                // per n lights
                auto data = iter.per_n_light();

                // number
                dst->setPassIterationCount(data.number());

                // number of lights
                dst->setLightCountPerIteration(data.num_lights());

                // light type
                switch (data.light_type())
                {
                case Script::MaterialSystem::LT_POINT:
                    dst->setIteratePerLight(true);
                    break;
                case Script::MaterialSystem::LT_DIRECTIONAL:
                    dst->setIteratePerLight(true, true, LightType::DIRECTIONAL);
                    break;
                case Script::MaterialSystem::LT_SPOT:
                    dst->setIteratePerLight(true, true, LightType::SPOTLIGHT);
                    break;
                default:
                    ret = T3D_ERR_RES_INVALID_PROPERTY;
                    T3D_LOG_ERROR(LOG_TAG_RESOURCE, "Invalid iteration data !");
                    break;
                }
            }
            else if (iter.has_number())
            {
                // number
                auto data = iter.number();

                dst->setPassIterationCount(data.number());
            }
        }

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePointSize(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_point_size())
        {
            dst->setPointSize(src->point_size().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePointSprites(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_point_sprites())
        {
            dst->setPointSpritesEnabled(src->point_sprites().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePointSizeAttenuation(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_point_size_attenuation())
        {
            auto attenuation = src->point_size_attenuation();

            if (attenuation.enable())
            {
                // enabled
                float32_t constant = attenuation.constant();
                float32_t linear = attenuation.linear();
                float32_t quadratic = attenuation.quadratic();
                dst->setPointAttenuation(true, constant, linear, quadratic);
            }
            else
            {
                // disabled
                dst->setPointAttenuation(false);
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePointSizeMin(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_point_size_min())
        {
            dst->setPointMinSize(src->point_size_min().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parsePointSizeMax(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        if (src->has_point_size_max())
        {
            dst->setPointMaxSize(src->point_size_max().value());
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseGPUProgramRef(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        TResult ret = T3D_OK;

        if (src->has_gpu_program_ref())
        {
            auto data = src->gpu_program_ref();

            // name
            auto header = data.header();
            GPUProgramRefPtr program = GPUProgramRef::create(header.name());
            auto refs = data.gpu_cbuffer_ref();

            // GPU constant buffer reference
            for (const MaterialSystem::GPUConstantBufferRef &ref : refs)
            {
                ret = parseGPUConstantBufferRef(&ref, program);
                T3D_CHECK_PARSING_RET(ret);
            }
        }

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseGPUConstantBufferRef(
        const MaterialSystem::GPUConstantBufferRef *src, GPUProgramRef *dst)
    {
        TResult ret = T3D_OK;

        do 
        {
            // name
            auto header = src->header();

            // slot
            uint32_t slot = src->slot();

            GPUConstBufferRefPtr buffer;
            ret = dst->addConstBufferRef(header.name(), slot, buffer);
            T3D_CHECK_PARSING_RET(ret);
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTextures(
        const MaterialSystem::Pass *src, Pass *dst)
    {
        TResult ret = T3D_OK;
        auto textures = src->textures();
        for (const MaterialSystem::TextureUnit &unit : textures)
        {
            ret = parseTextureUnit(&unit, dst);
            T3D_CHECK_PARSING_RET(ret);
        }

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTextureUnit(
        const MaterialSystem::TextureUnit *src, Pass *dst)
    {
        TResult ret = T3D_OK;

        do 
        {
            auto header = src->header();

            TextureUnitPtr unit;
            ret = dst->addTextureUnit(header.name(), unit);
            T3D_CHECK_PARSING_RET(ret);

            // texture_alias
            ret = parseTextureAlias(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // texture
            ret = parseTexture(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // anim_texture
            ret = parseAnimTexture(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // cubic_texture
            ret = parseCubicTexture(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // binding_type
            ret = parseBindingType(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // tex_coord_set
            ret = parseTexCoordSet(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // color_op
            ret = parseColorOp(src, unit);
            T3D_CHECK_PARSING_RET(ret);

            // sampler_ref
            ret = parseSamplerRef(src, unit);
            T3D_CHECK_PARSING_RET(ret);
        } while (0);

        return ret;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTextureAlias(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_texture_alias())
        {
            // auto alias = src->texture_alias();
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTexture(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_texture())
        {
            // texture name
            auto texture = src->texture();

            // texture type
            TextureType texType = (TextureType)texture.type();
            // mipmaps
            uint32_t mipmaps = texture.mipmaps();
            // alpha
            bool alpha = texture.alpha();
            // gamma
            bool gamma = texture.gamma();

            dst->setTextureName(texture.name());
        }
        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseAnimTexture(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_anim_texture())
        {
            auto animTex = src->anim_texture();

            if (animTex.has_simple())
            {
                // simple format
                auto simple = animTex.simple();

                // base_name
                auto name = simple.base_name();
                // num_frames
                auto frames = simple.num_frames();
                // duration
                auto duration = simple.duration();
            }
            else if (animTex.has_complex())
            {
                // complex format
                auto complex = animTex.complex();

                // duration
                auto duration = complex.duration();
                // frames
                auto frames = complex.frames();
                for (const String &name : frames)
                {

                }
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseCubicTexture(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_cubic_texture())
        {
            auto cubicTex = src->cubic_texture();

            // combinedUVW | separateUV
            auto mode = cubicTex.cubic_texture_mode();

            if (cubicTex.has_simple())
            {
                // simple format
                auto simple = cubicTex.simple();

                // base_name
                auto name = simple.base_name();
            }
            else if (cubicTex.has_complex())
            {
                // complex format
                auto complex = cubicTex.complex();

                // front
                auto front = complex.front();
                // back
                auto back = complex.back();
                // left
                auto left = complex.left();
                // right
                auto right = complex.right();
                // up
                auto up = complex.up();
                // down
                auto down = complex.down();
            }
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseBindingType(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_binding_type())
        {
            // binding_type
            auto type = src->binding_type().value();
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseTexCoordSet(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_tex_coord_set())
        {
            uint32_t index = src->tex_coord_set().value();
        }

        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseColorOp(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_color_op())
        {
            BlendType type = (BlendType)(src->color_op().value());
        }
        return T3D_OK;
    }

    //--------------------------------------------------------------------------

    TResult BinMaterialReader::parseSamplerRef(
        const MaterialSystem::TextureUnit *src, TextureUnit *dst)
    {
        if (src->has_sampler_ref())
        {
            dst->setSampler(src->sampler_ref().value());
        }

        return T3D_OK;
    }
}

