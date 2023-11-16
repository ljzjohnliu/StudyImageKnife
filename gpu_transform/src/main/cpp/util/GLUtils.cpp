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
// Created on 2022/12/21.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,

#include <malloc.h>
#include <stddef.h>
#include "DebugLog.h"
#include "GLUtils.h"

GLuint GLUtils::LoadShader(GLenum shaderType, const char *pSource)
{
    GLuint shader = 0;
    shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc((size_t)infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint GLUtils::CreateProgram(const char *pVertexShaderSource,
                              const char *pFragShaderSource,
                              GLuint &vertexShaderHandle,
                              GLuint &fragShaderHandle)
{
    GLuint program = 0;
    vertexShaderHandle = LoadShader(GL_VERTEX_SHADER, pVertexShaderSource);
    if (!vertexShaderHandle) return program;
    fragShaderHandle = LoadShader(GL_FRAGMENT_SHADER, pFragShaderSource);
    if (!fragShaderHandle) return program;
    
    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShaderHandle);
        CheckGLError("glAttachShader");
        glAttachShader(program, fragShaderHandle);
        CheckGLError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        
        glDetachShader(program, vertexShaderHandle);
        glDeleteShader(vertexShaderHandle);
        vertexShaderHandle = 0;
        glDetachShader(program, fragShaderHandle);
        glDeleteShader(fragShaderHandle);
        fragShaderHandle = 0;
        
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc((size_t)bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void GLUtils::CheckGLError(const char *pGLOperation)
{
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("GLUtils::CheckGLError GL Operation %{public}s() glError (0x%x)\n", pGLOperation, error);
    }
}
