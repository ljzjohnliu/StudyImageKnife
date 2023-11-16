/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created on 2022/12/20.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,


#include <js_native_api.h>
#include <js_native_api_types.h>
#include <node_api.h>
#include "DebugLog.h"
#include "native_common.h"
#include "EGLRender.h"

static napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("EglRenderInit", EGLRender::RenderInit),
        DECLARE_NAPI_FUNCTION("EglRenderSetImageData", EGLRender::RenderSetData),
        DECLARE_NAPI_FUNCTION("EglPixelMapSurface", EGLRender::GetPixelMapOfSurface),
        DECLARE_NAPI_FUNCTION("EglRenderSetIntParams", EGLRender::RenderSetIntParams),
        DECLARE_NAPI_FUNCTION("EglIsInit", EGLRender::EGLIsInit),
        DECLARE_NAPI_FUNCTION("EglDestroy", EGLRender::DestroyGlesEnv),
    
        DECLARE_NAPI_FUNCTION("EglUseProgram", EGLRender::StartUseProgram),
        DECLARE_NAPI_FUNCTION("EglRendering", EGLRender::Rendering),
    
        DECLARE_NAPI_FUNCTION("EglUniform1i", EGLRender::RenderGlUniform1i),
        DECLARE_NAPI_FUNCTION("EglUniform1f", EGLRender::RenderGlUniform1f),
        DECLARE_NAPI_FUNCTION("EglUniform2fv", EGLRender::RenderGlUniform2fv),
    
        DECLARE_NAPI_FUNCTION("EglSetTypeArrayOfFloat", EGLRender::setTypeArrayOfFloat),
    
        DECLARE_NAPI_FUNCTION("EglSetTypeArrayOfMatrix3f", EGLRender::setTypeArrayOfMatrix3f),
        DECLARE_NAPI_FUNCTION("EglSetTypeArrayOfMatrix4f", EGLRender::setTypeArrayOfMatrix4f),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

/**
 * Napi Module define
 */
static napi_module nativeGpuModule = {
    .nm_version =1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "nativeGpu",
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&nativeGpuModule);
}