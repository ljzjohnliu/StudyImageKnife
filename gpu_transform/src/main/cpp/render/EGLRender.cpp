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

#include "EGLRender.h"

#include <js_native_api.h>
#include <mutex>
#include <napi/native_api.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <stdint.h>
#include <js_native_api_types.h>
#include "native_common.h"
#include "DebugLog.h"
#include "constant/constant_shape.h"
#include "GLUtils.h"
#include "../util/NapiUtil.h"


const int PARAM_TYPE_SHADER_INDEX = 300;

const int VERTEX_POS_LOC = 0;
const int TEXTURE_POS_LOC = 1;

const int32_t STR_DEFAULT_SIZE = 1024;
EGLRender *EGLRender::sInstance = nullptr;

// 顶点坐标
const static GLfloat vVertices[] = {
    -1.0f, -1.0f, 0.0f, // bottom left
    1.0f, -1.0f, 0.0f, // bottom right
    -1.0f, 1.0f, 0.0f, // top left
    1.0f, 1.0f, 0.0f, // top right
};

// 正常纹理坐标
const static GLfloat vTexCoors[] = {
    0.0f, 1.0f, // bottom left
    1.0f, 1.0f, // bottom right
    0.0f, 0.0f, // top left
    1.0f, 0.0f, // top right
};

// fbo 纹理坐标与正常纹理方向不同（上下镜像）
const static GLfloat vFboTexCoors[] = {
    0.0f, 0.0f, // bottom left
    1.0f, 0.0f, // bottom right
    0.0f, 1.0f, // top left
    1.0f, 1.0f, // top right
};

const static GLushort indices[] = { 0, 1, 2, 1, 3, 2 };
std::mutex mtx;

EGLRender* EGLRender::GetInstance()
{
    mtx.lock();
    if (sInstance == nullptr) {
        sInstance = new EGLRender();
    }
    mtx.unlock();
    return sInstance;
}

napi_value EGLRender::RenderInit(napi_env env, napi_callback_info info)
{
    napi_value exports;
    NAPI_CALL(env, napi_create_object(env, &exports));
    napi_property_descriptor desc[] = {};
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    EGLRender::GetInstance() ->Init();
    return exports;
}


napi_value EGLRender::RenderSetData(napi_env env, napi_callback_info info)
{
    size_t argc = 3;
    napi_value args[3] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    void* buffer;
    size_t bufferLength;
    napi_status buffStatus = napi_get_arraybuffer_info(env, args[0], &buffer, &bufferLength);
    if (buffStatus != napi_ok) {
        return nullptr;
    }
    
    uint8_t* uint8_buf = reinterpret_cast<uint8_t *>(buffer);
    uint32_t width;
    napi_status wStatus = napi_get_value_uint32(env, args[1], &width);
    if (wStatus != napi_ok) {
        return nullptr;
    }
    uint32_t height;
    napi_status hStatus = napi_get_value_uint32(env, args[2], &height);
    if (hStatus != napi_ok) {
        return nullptr;
    }
    
    EGLRender::GetInstance() -> SetImageData(uint8_buf, width, height);
    return nullptr;
}

napi_value EGLRender::RenderSetIntParams(napi_env env, napi_callback_info info)
{
    LOGI("gl--> RenderSetIntParams start");
    size_t argc = 2;
    napi_value args[2] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    uint32_t type;
    napi_status tStatus = napi_get_value_uint32(env, args[0], &type);
    if (tStatus != napi_ok) {
        return nullptr;
    }
    uint32_t param;
    napi_status pStatus = napi_get_value_uint32(env, args[1], &param);
    if (pStatus != napi_ok) {
        return nullptr;
    }
    EGLRender::GetInstance() -> SetIntParams(type, param);
    return nullptr;
}

napi_value EGLRender::GetPixelMapOfSurface(napi_env env, napi_callback_info info)
{
    size_t argc = 4;
    napi_value args[4] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    uint32_t x;
    napi_status xStatus = napi_get_value_uint32(env, args[0], &x);
    if (xStatus != napi_ok) {
        return nullptr;
    }
    
    uint32_t y;
    napi_status yStatus = napi_get_value_uint32(env, args[1], &y);
    if (yStatus != napi_ok) {
        return nullptr;
    }

    uint32_t surfaceWidth;
    napi_status swStatus = napi_get_value_uint32(env, args[2], &surfaceWidth);
    if (swStatus != napi_ok) {
        return nullptr;
    }
    
    uint32_t surfaceHeight;
    napi_status shStatus = napi_get_value_uint32(env, args[3], &surfaceHeight);
    if (shStatus != napi_ok) {
        return nullptr;
    }
    
    uint8_t* pixels = (uint8_t*) malloc(surfaceWidth * surfaceHeight * DEFAULT_FOUR);
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, surfaceWidth, surfaceHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    NativeImageUtil::Flip(&pixels, surfaceWidth, surfaceHeight);
    napi_value array;
    int byte_length = surfaceWidth * surfaceHeight * DEFAULT_FOUR;
    if (!NativeImageUtil::CreateArrayBuffer(env, pixels, byte_length, &array)) {
        LOGI("gl--> GetPixelMapOfSurface error");
    }
    free(pixels);
    return array;
}

napi_value EGLRender::EGLIsInit(napi_env env, napi_callback_info info)
{
    napi_value isInit;
    int32_t value;
    if (EGLRender::GetInstance() -> m_IsGLContextReady) {
        value = 1;
    } else {
        value = 0;
    }
    napi_status status = napi_create_int32(env, value, &isInit);
    if (status != napi_ok) {
        return nullptr;
    }
    return isInit;
}

napi_value EGLRender::DestroyGlesEnv(napi_env env, napi_callback_info info)
{
    EGLRender::GetInstance() -> UnInit();
    return nullptr;
}

napi_value EGLRender::StartUseProgram(napi_env env, napi_callback_info info)
{
    EGLRender::GetInstance() -> UseProgram();
    return nullptr;
}

napi_value EGLRender::Rendering(napi_env env, napi_callback_info info)
{
    // 渲染
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);
    glBindVertexArray(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    return nullptr;
}

napi_value EGLRender::setTypeArrayOfFloat(napi_env env, napi_callback_info info)
{
    size_t argc = 3;
    napi_value args[3] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    std::string locationContent;
    NapiUtil::JsValueToString(env, args[0], STR_DEFAULT_SIZE, locationContent);
    
    char* location = (char*)locationContent.c_str();
    
    std::string content;
    NapiUtil::JsValueToString(env, args[1], STR_DEFAULT_SIZE, content);
    
    char* key = (char*)content.c_str();
    
    napi_typedarray_type dataType = napi_float32_array;
    void* buffer;
    size_t bufferLength;
    size_t byte_offset;
    napi_status buffStatus = napi_get_typedarray_info(env,
        args[2], &dataType, &bufferLength, &buffer, &args[2], &byte_offset);
    if (buffStatus != napi_ok) {
        return nullptr;
    }
    
    float* value = reinterpret_cast<float *>(buffer);
    
    int uniformType;
    
    if (strcmp(key, "glUniform2fv") == 0) {
        uniformType = UNIFORM_TYPE_2FV;
    } else if (strcmp(key, "glUniform3fv") == 0) {
        uniformType = UNIFORM_TYPE_3FV;
    } else if (strcmp(key, "glUniform4fv") == 0) {
        uniformType = UNIFORM_TYPE_4FV;
    } else if (strcmp(key, "glUniform1fv") == 0) {
        uniformType = UNIFORM_TYPE_FV;
    } else if (strcmp(key, "glUniform2f") == 0) {
        uniformType = UNIFORM_TYPE_2F;
    }
    
    EGLRender::GetInstance() -> GlUniformArray(location, value, uniformType);
    return nullptr;
}

napi_value EGLRender::setTypeArrayOfMatrix3f(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    std::string locationContent;
    NapiUtil::JsValueToString(env, args[0], STR_DEFAULT_SIZE, locationContent);
    
    char* location = (char*)locationContent.c_str();
    
    napi_typedarray_type dataType = napi_float32_array;
    void* buffer;
    size_t bufferLength;
    size_t byte_offset;
    napi_status buffStatus = napi_get_typedarray_info(env,
        args[1], &dataType, &bufferLength, &buffer, &args[1], &byte_offset);
    if (buffStatus != napi_ok) {
        return nullptr;
    }
    
    float* value = reinterpret_cast<float *>(buffer);
    
    EGLRender::GetInstance() -> GlUniformMatrix(location, value, UNIFORM_TYPE_THREE);
    
    return nullptr;
}

napi_value EGLRender::setTypeArrayOfMatrix4f(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    std::string locationContent;
    NapiUtil::JsValueToString(env, args[0], STR_DEFAULT_SIZE, locationContent);
    
    char* location = (char*)locationContent.c_str();
    
    napi_typedarray_type dataType = napi_float32_array;
    void* buffer;
    size_t bufferLength;
    size_t byte_offset;
    napi_status buffStatus = napi_get_typedarray_info(env,
        args[1], &dataType, &bufferLength, &buffer, &args[1], &byte_offset);
    if (buffStatus != napi_ok) {
        return nullptr;
    }
    
    float* value = reinterpret_cast<float *>(buffer);
    
    EGLRender::GetInstance() -> GlUniformMatrix(location, value, UNIFORM_TYPE_FOUR);
    return nullptr;
}

napi_value EGLRender::RenderGlUniform1i(napi_env env, napi_callback_info info)
{
    // int
    size_t argc = 2;
    napi_value args[2] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    std::string content;
    NapiUtil::JsValueToString(env, args[0], STR_DEFAULT_SIZE, content);
    uint32_t value;
    napi_status status = napi_get_value_uint32(env, args[1], &value);
    if (status != napi_ok) {
        return nullptr;
    }
    EGLRender::GetInstance() -> GlUniform((char*)content.c_str(), value, UNIFORM_TYPE_ZERO);
    return nullptr;
}

napi_value EGLRender::RenderGlUniform1f(napi_env env, napi_callback_info info)
{
    // float
    size_t argc = 2;
    napi_value args[2] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    std::string content;
    NapiUtil::JsValueToString(env, args[0], STR_DEFAULT_SIZE, content);
    double value;
    napi_status status = napi_get_value_double(env, args[1], &value);
    if (status != napi_ok) {
        return nullptr;
    }
    EGLRender::GetInstance() -> GlUniform((char*)content.c_str(), value, UNIFORM_TYPE_ONE);
    return nullptr;
}

napi_value EGLRender::RenderGlUniform2fv(napi_env env, napi_callback_info info)
{
    // float 数组
    size_t argc = 3;
    napi_value args[3] = { nullptr };
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    std::string content;
    NapiUtil::JsValueToString(env, args[0], STR_DEFAULT_SIZE, content);
    
    double value;
    napi_status status = napi_get_value_double(env, args[1], &value);
    if (status != napi_ok) {
        return nullptr;
    }
    double value2;
    napi_status status2 = napi_get_value_double(env, args[2], &value2);
    if (status2 != napi_ok) {
        return nullptr;
    }
    
    float vce2[2];
    vce2[0] = value;
    vce2[1] = value2;
    EGLRender::GetInstance() -> GlUniformArray((char*)content.c_str(), vce2, UNIFORM_TYPE_2FV_SIZE);
    return nullptr;
}

EGLRender::EGLRender()
{
    m_ImageTextureId = GL_NONE;
    m_FboTextureId = GL_NONE;
    m_SamplerLoc = GL_NONE;
    m_TexSizeLoc = GL_NONE;
    
    m_FboId = GL_NONE;
    m_ProgramObj = GL_NONE;
    m_VertexShader = GL_NONE;
    m_FragmentShader = GL_NONE;
    m_eglDisplay = nullptr;
    m_IsGLContextReady = false;
    m_ShaderIndex = 0;
}
EGLRender::~EGLRender()
{
}
void EGLRender::TexturesInit()
{
    glGenTextures(1, &m_ImageTextureId);    // 生成纹理名称
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);  // 允许建立一个绑定到目标纹理的有名称的纹理
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glGenTextures(1, &m_FboTextureId);
    glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}
void EGLRender::Init()
{
    if (CreateGlEnv() == 0) {
        m_IsGLContextReady = true;
    }
    if (!m_IsGLContextReady) {
        return;
    }
    EGLRender::GetInstance() -> TexturesInit();
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr0, m_VertexShader,
                                          m_FragmentShader);
    if (!m_ProgramObj) {
        GLUtils::CheckGLError("Create Program");
        return;
    }
    glGenBuffers(DEFAULT_THREE, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[DEFAULT_ZERO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[DEFAULT_ONE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vFboTexCoors), vTexCoors, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[DEFAULT_TWO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glGenVertexArrays(DEFAULT_ONE, m_VaoIds);
    glBindVertexArray(m_VaoIds[DEFAULT_ZERO]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[DEFAULT_ZERO]);
    glEnableVertexAttribArray(VERTEX_POS_LOC);
    glVertexAttribPointer(VERTEX_POS_LOC,
                          DEFAULT_THREE,
                          GL_FLOAT, GL_FALSE,
                          DEFAULT_THREE * sizeof(GLfloat),
                          (const void *)DEFAULT_ZERO);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[DEFAULT_ONE]);
    glEnableVertexAttribArray(TEXTURE_POS_LOC);
    glVertexAttribPointer(TEXTURE_POS_LOC,
                          DEFAULT_TWO,
                          GL_FLOAT,
                          GL_FALSE,
                          DEFAULT_TWO * sizeof(GLfloat),
                          (const void *)DEFAULT_ZERO);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[DEFAULT_TWO]);
    glBindVertexArray(GL_NONE);
}

int EGLRender::CreateGlEnv()
{
    const EGLint confAttr[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR, EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16, EGL_STENCIL_SIZE, 8, EGL_NONE
    };
    const EGLint ctxAttr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    const EGLint surfaceAttr[] = {
        EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE
    };
    EGLint eglMajVers, eglMinVers;
    EGLint numConfigs;
    int resultCode = 0;
    do {
        m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_eglDisplay == EGL_NO_DISPLAY) {
            resultCode = -1;
            break;
        }
        // 初始化 egl 方法
        if (!eglInitialize(m_eglDisplay, &eglMajVers, &eglMinVers)) {
            resultCode = -1;
            break;
        }
        // 获取 EGLConfig 对象，确定渲染表面的配置信息
        if (!eglChooseConfig(m_eglDisplay, confAttr, &m_eglConf, 1, &numConfigs)) {
            resultCode = -1;
            break;
        }
        // 创建渲染表面 EGLSurface 使用 eglCreateBufferSurface 创建屏幕外渲染区域
        m_eglSurface = eglCreatePbufferSurface(m_eglDisplay, m_eglConf, surfaceAttr);
        if (m_eglSurface == EGL_NO_SURFACE) {
            LOGI("gl-->::CreateGlesEnv happen default error");
            break;
        }
        // 创建渲染上下文 EGLContext
        m_eglCtx = eglCreateContext(m_eglDisplay, m_eglConf, EGL_NO_CONTEXT, ctxAttr);
        if (m_eglCtx == EGL_NO_CONTEXT) {
            EGLint error = eglGetError();
            if (error == EGL_BAD_CONFIG) {
                resultCode = -1;
                break;
            }
        }
        // 绑定上下文
        if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglCtx)) {
            resultCode = -1;
            break;
        }
    } while (false);
    return resultCode;
}

void EGLRender::SetImageData(uint8_t *pData, int width, int height)
{
    if (pData && m_IsGLContextReady) {
        if (m_RenderImage.ppPlane[0]) {
            NativeImageUtil::FreeNativeImage(&m_RenderImage);
            m_RenderImage.ppPlane[0] = nullptr;
        }
        m_RenderImage.width = width;
        m_RenderImage.height = height;
        m_RenderImage.format = IMAGE_FORMAT_RGBA;
        NativeImageUtil::AllocNativeImage(&m_RenderImage);
        memcpy(m_RenderImage.ppPlane[0], pData, width * height * DEFAULT_FOUR);
        
        glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     m_RenderImage.width,
                     m_RenderImage.height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     m_RenderImage.ppPlane[0]);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        
        if (m_FboId == GL_NONE) {
            // Create FBO
            glGenFramebuffers(1, &m_FboId);
            glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
            glBindTexture(GL_TEXTURE_2D, m_FboTextureId);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTextureId, 0);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         m_RenderImage.width,
                         m_RenderImage.height,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         nullptr);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                LOGI("gl--> EGLRender ::SetImageData glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
            }
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
        }
        LOGI("gl--> :: SetImageData end");
    }
}

void EGLRender::SetIntParams(int paramType, int param)
{
    LOGI("gl--> EGLRender::SetIntParams paramType = %{public}d,param = %{public}d", paramType, param);
    switch (paramType) {
        case PARAM_TYPE_SHADER_INDEX: {
            if (param >= 0) {
                if (m_ProgramObj) {
                    glDeleteProgram(m_ProgramObj);
                    m_ProgramObj = GL_NONE;
                }
                const char* vShader[1];
                vShader[0] = vShaderStr;
                const char* fShader[1];
                switch (param) {
                    case SHADER_TYPE_KUWAHARA: {
                        fShader[0] = fShaderStr3;
                        break;
                    }
                    case SHADER_TYPE_SWIRL: {
                        fShader[0] = swirlFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_BRIGHT: {
                        fShader[0] = brightFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_CONTRAST: {
                        fShader[0] = contrastFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_INVERT: {
                        fShader[0] = invertFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_PIXELATION: {
                        fShader[0] = pixelFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_SEPIA: {
                        fShader[0] = colorMatrixFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_SKETCH: {
                        fShader[0] = sketchShaderStr;
                        vShader[0] = v3x3ShaderStr;
                        break;
                    }
                    case SHADER_TYPE_TOON: {
                        fShader[0] = toonFShaderStr;
                        vShader[0] = v3x3ShaderStr;
                        break;
                    }
                    case SHADER_TYPE_VIGNETTE: {
                        fShader[0] = vignetteFShaderStr;
                        break;
                    }
                    case SHADER_TYPE_GRAYSCALE: {
                        fShader[0] = grayScaleShaderStr;
                        break;
                    }
                    case SHADER_TYPE_BLUR: {
                        fShader[0] = blurShaderStr;
                        break;
                    }
                    default:
                        vShader[0] = vShaderStr;
                        break;
                }
                m_ProgramObj = GLUtils::CreateProgram(vShader[0], fShader[0], m_VertexShader,
                                                      m_FragmentShader);
                if (!m_ProgramObj) {
                    GLUtils::CheckGLError("Create Program");
                    LOGI("gl--> EGLRender::SetIntParams Could not create program.");
                    return;
                }
                m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
                m_TexSizeLoc = glGetUniformLocation(m_ProgramObj, "u_texSize");
            }
        }
            break;
        default:
            break;
    }
}

void EGLRender::UseProgram()
{
    if (m_ProgramObj == GL_NONE) {
        return;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);
    
    // DO FBO off screen rendering
    glUseProgram(m_ProgramObj);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    
    glBindVertexArray(m_VaoIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ImageTextureId);
    glUniform1i(m_SamplerLoc, 0);
    
    if (m_TexSizeLoc > -1) {
        GLfloat size[2];
        size[0] = m_RenderImage.width;
        size[1] = m_RenderImage.height;
        glUniform2f(m_TexSizeLoc, size[0], size[1]);
    }
}

void EGLRender::GlUniform(char* location, float value, int unType)
{
    GLint ll = glGetUniformLocation(m_ProgramObj, location);
    switch (unType) {
        case UNIFORM_TYPE_ZERO:
            glUniform1i(ll, (int)value);
            break;
        case UNIFORM_TYPE_ONE:
            glUniform1f(ll, value);
            break;
        default:
            break;
    }
}

void EGLRender::GlUniformArray(char* location, float* value, int unType)
{
    GLint ll = glGetUniformLocation(m_ProgramObj, location);
    switch (unType) {
        case UNIFORM_TYPE_2FV_SIZE:
            GLfloat vec2[2];
            vec2[0] = value[0] * m_RenderImage.width;
            vec2[1] = value[1] * m_RenderImage.height;
            glUniform2fv(ll, 1, vec2);
            break;
        case UNIFORM_TYPE_2F:
            glUniform2f(ll, value[0], value[1]);
            break;
        case UNIFORM_TYPE_FV:
            glUniform1fv(ll, 1, value);
            break;
        case UNIFORM_TYPE_2FV:
            glUniform2fv(ll, 1, value);
            break;
        case UNIFORM_TYPE_3FV:
            glUniform3fv(ll, 1, value);
            break;
        case UNIFORM_TYPE_4FV:
            glUniform4fv(ll, 1, value);
            break;
        default:
            break;
    }
}

void EGLRender::GlUniformMatrix(char* location, float* value, int unType)
{
    GLint ll = glGetUniformLocation(m_ProgramObj, location);
    switch (unType) {
        case UNIFORM_TYPE_THREE:
            glUniformMatrix3fv(ll, 1, false, value);
            break;
        case UNIFORM_TYPE_FOUR:
            glUniformMatrix4fv(ll, 1, false, value);
            break;
        default:
            break;
    }
}

void EGLRender::UnInit()
{
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        m_ProgramObj = GL_NONE;
    }
    
    if (m_ImageTextureId) {
        glDeleteTextures(DEFAULT_ONE, &m_ImageTextureId);
        m_ImageTextureId = GL_NONE;
    }
    
    if (m_FboTextureId) {
        glDeleteTextures(DEFAULT_ONE, &m_FboTextureId);
        m_FboTextureId = GL_NONE;
    }
    
    if (m_VboIds[DEFAULT_ZERO]) {
        glDeleteBuffers(DEFAULT_THREE, m_VboIds);
        m_VboIds[DEFAULT_ZERO] = GL_NONE;
        m_VboIds[DEFAULT_ONE] = GL_NONE;
        m_VboIds[DEFAULT_TWO] = GL_NONE;
    }
    
    if (m_VaoIds[DEFAULT_ZERO]) {
        glDeleteVertexArrays(DEFAULT_ONE, m_VaoIds);
        m_VaoIds[DEFAULT_ZERO] = GL_NONE;
    }
    
    if (m_FboId) {
        glDeleteFramebuffers(1, &m_FboId);
        m_FboId = GL_NONE;
    }
    if (m_IsGLContextReady) {
        DestroyGl();
        m_IsGLContextReady = false;
    }
}
void EGLRender::DestroyGl()
{
    // 释放 EGL 环境
    if (m_eglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(m_eglDisplay, m_eglCtx);
        eglDestroySurface(m_eglDisplay, m_eglSurface);
        eglReleaseThread();
        eglTerminate(m_eglDisplay);
    }
    
    m_eglDisplay = EGL_NO_DISPLAY;
    m_eglSurface = EGL_NO_SURFACE;
    m_eglCtx = EGL_NO_CONTEXT;
}
