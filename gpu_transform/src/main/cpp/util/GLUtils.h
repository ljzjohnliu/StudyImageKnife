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

#ifndef GPU_ImageETS_GLUtils_H
#define GPU_ImageETS_GLUtils_H

#include <GLES3/gl3.h>

class GLUtils {
public:
        static GLuint LoadShader(GLenum shaderType, const char *pSource);
        static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                                        GLuint &vertexShaderHandle,
                                        GLuint &fragShaderHandle);
        static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource);
        static void CheckGLError(const char *pGLOperation);
};

#endif // GPU_ImageETS_GLUtils_H
