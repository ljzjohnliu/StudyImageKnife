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

#ifndef GPU_ImageETS_EGLRender_H
#define GPU_ImageETS_EGLRender_H
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <js_native_api.h>
#include <napi/native_api.h>
#include <stdint.h>
#include "NativeImage.h"

const int EGL_FEATURE_NUM = 7;
class EGLRender {
private:
            EGLRender();
            ~EGLRender();
public:
            static void DestroyRender()
            {
                if (sInstance) {
                    delete sInstance;
                    sInstance = nullptr;
                }
            }
            static EGLRender* GetInstance();
            static napi_value RenderInit(napi_env env, napi_callback_info info);
            static napi_value RenderSetData(napi_env env, napi_callback_info info);
            static napi_value GetPixelMapOfSurface(napi_env env, napi_callback_info info);
            static napi_value RenderSetIntParams(napi_env env, napi_callback_info info);
            static napi_value EGLIsInit(napi_env env, napi_callback_info info);
    
            static napi_value DestroyGlesEnv(napi_env env, napi_callback_info info);
    
            static napi_value StartUseProgram(napi_env env, napi_callback_info info);
    
            static napi_value Rendering(napi_env env, napi_callback_info info);
    
            static napi_value RenderGlUniform1i(napi_env env, napi_callback_info info);
            static napi_value RenderGlUniform1f(napi_env env, napi_callback_info info);
            static napi_value RenderGlUniform2fv(napi_env env, napi_callback_info info);
            static napi_value setTypeArrayOfFloat(napi_env env, napi_callback_info info);
    
            static napi_value setTypeArrayOfMatrix3f(napi_env env, napi_callback_info info);
            static napi_value setTypeArrayOfMatrix4f(napi_env env, napi_callback_info info);
            
            void Init();
            void TexturesInit();
            int CreateGlEnv();
    
            void SetImageData(uint8_t *pData, int width, int height);
            
            void SetIntParams(int paramType, int param);
            
            void UseProgram();
            
            void Draw();
    
            void GlUniform(char* location, float value, int unType);
            void GlUniformArray(char* location, float* value, int unType);
    
            void GlUniformMatrix(char* location, float* value, int unType);
            
            void DestroyGl();
    
            void UnInit();
        
private:
            static EGLRender* sInstance;
            GLuint m_ImageTextureId;
            GLuint m_FboTextureId;
            GLuint m_FboId;
            GLuint m_VaoIds[1] = {GL_NONE};
            GLuint m_VboIds[3] = {GL_NONE};
            GLint m_SamplerLoc;
            GLint m_TexSizeLoc;
    
            NativeImage m_RenderImage;
            GLuint m_ProgramObj;
            GLuint m_VertexShader;
            GLuint m_FragmentShader;
    
            EGLConfig m_eglConf;
            EGLSurface m_eglSurface;
            EGLContext m_eglCtx;
            EGLDisplay m_eglDisplay;
            bool        m_IsGLContextReady;
            const char* m_fShaderStrs[EGL_FEATURE_NUM];
            int         m_ShaderIndex;
};

#endif // GPU_ImageETS_EGLRender_H
