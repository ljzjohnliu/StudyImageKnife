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
// Created on 2022/12/16.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,

#ifndef GPU_ImageETS_constant_shape_H
#define GPU_ImageETS_constant_shape_H


const int UNIFORM_TYPE_2FV_SIZE = 10;
const int UNIFORM_TYPE_FV = 1;
const int UNIFORM_TYPE_2FV = 2;
const int UNIFORM_TYPE_3FV = 3;
const int UNIFORM_TYPE_4FV = 4;
const int UNIFORM_TYPE_2F = 21;

const int DEFAULT_ZERO = 0;
const int DEFAULT_ONE = 1;
const int DEFAULT_TWO = 2;
const int DEFAULT_THREE = 3;
const int DEFAULT_FOUR = 4;
const float DEFAULT_ONE_HALF = 1.5;

const int UNIFORM_TYPE_ZERO = 0;
const int UNIFORM_TYPE_ONE = 1;
const int UNIFORM_TYPE_THREE = 3;
const int UNIFORM_TYPE_FOUR = 4;


const int SHADER_TYPE_BRIGHT = 0;
const int SHADER_TYPE_CONTRAST = 1;
const int SHADER_TYPE_INVERT = 2;
const int SHADER_TYPE_PIXELATION = 3;
const int SHADER_TYPE_KUWAHARA = 4;
const int SHADER_TYPE_SEPIA = 5;
const int SHADER_TYPE_SKETCH = 6;
const int SHADER_TYPE_SWIRL = 7;
const int SHADER_TYPE_TOON = 8;
const int SHADER_TYPE_VIGNETTE = 9;
const int SHADER_TYPE_GRAYSCALE = 10;
const int SHADER_TYPE_BLUR = 12;


const char vShaderStr[] =
        "#version 300 es                                    \n"
        "layout(location = 0) in vec4 a_position;           \n"
        "layout(location = 1) in vec2 a_texCoord;           \n"
        "out vec2 v_texCoord;                               \n"
        "void main()                                        \n"
        "{                                                  \n"
        "    gl_Position = a_position;                      \n"
        "    v_texCoord = a_texCoord;                       \n"
        "}                                                  \n";

const char fShaderStr0[] =
        "#version 300 es                                    \n"
        "precision mediump float;                           \n"
        "in vec2 v_texCoord;                                \n"
        "layout(location = 0) out vec4 outColor;            \n"
        "uniform sampler2D s_TextureMap;                    \n"
        "void main()                                        \n"
        "{                                                  \n"
            "outColor = texture(s_TextureMap,v_texCoord);   \n"
        "}                                                  \n";

const char v3x3ShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "layout(location = 0) in vec4 a_position;\n"
        "layout(location = 1) in vec2 a_texCoord;\n"
        "uniform highp float texelWidth;\n"
        "uniform highp float texelHeight;\n"
        "out vec2 v_texCoord;\n"
        "out vec2 leftTextureCoordinate;\n"
        "out vec2 rightTextureCoordinate;\n"
        "out vec2 topTextureCoordinate;\n"
        "out vec2 topLeftTextureCoordinate;\n"
        "out vec2 topRightTextureCoordinate;\n"
        "out vec2 bottomTextureCoordinate;\n"
        "out vec2 bottomLeftTextureCoordinate;\n"
        "out vec2 bottomRightTextureCoordinate;\n"
        "void main()\n"
        "{\n"
                "gl_Position = a_position;\n"
                "vec2 widthStep = vec2(texelWidth, 0.0);\n"
                "vec2 heightStep = vec2(0.0, texelHeight);\n"
                "vec2 widthHeightStep = vec2(texelWidth, texelHeight);\n"
                "vec2 widthNegativeHeightStep = vec2(texelWidth, -texelHeight);\n"
                "v_texCoord = a_texCoord;\n"
                "leftTextureCoordinate = a_texCoord - widthStep;\n"
                "rightTextureCoordinate = a_texCoord + widthStep;\n"
                "\n"
                "topTextureCoordinate = a_texCoord - heightStep;\n"
                "topLeftTextureCoordinate = a_texCoord - widthHeightStep;\n"
                "topRightTextureCoordinate = a_texCoord + widthNegativeHeightStep;\n"
                "\n"
                "bottomTextureCoordinate = a_texCoord +heightStep;\n"
                "bottomLeftTextureCoordinate = a_texCoord - widthNegativeHeightStep;\n"
                "bottomRightTextureCoordinate = a_texCoord + widthHeightStep;\n"
        "}";

// kuwahara
const char fShaderStr3[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "in vec2 v_texCoord;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "uniform vec2 u_texSize;\n"
        "uniform highp float radius;\n"
        "const vec2 src_size = vec2(1.0 / 768.0, 1.0 / 1024.0);\n"
        "void main() {\n"
                "vec2 uv = v_texCoord;\n"
                "float n = float((radius + 1.0) * (radius + 1.0));\n"
                "int i ; int j ;\n"
                "vec3 m0 = vec3(0.0); vec3 m1 = vec3(0.0); vec3 m2 = vec3(0.0); vec3 m3 = vec3(0.0);\n"
                "vec3 s0 = vec3(0.0); vec3 s1 = vec3(0.0); vec3 s2 = vec3(0.0); vec3 s3 = vec3(0.0);\n"
                "vec3 c;\n"
                "for (j = -int(radius); j <=0; ++j) {\n"
                    "for (i = -int(radius); i <=0; ++i) {\n"
                        "c = texture(s_TextureMap,uv + vec2(i,j) * src_size).rgb;\n"
                        "m0 += c;\n"
                        "s0 += c * c;\n"
                    "}\n"
                "}\n"
                "for (j = -int(radius); j <=0; ++j) {\n"
                    "for (i =0; i <=int(radius); ++i) {\n"
                        "c = texture(s_TextureMap,uv + vec2(i,j) * src_size).rgb;\n"
                        "m1 += c;\n"
                        "s1 += c * c;\n"
                    "}\n"
                "}\n"
                "for (j = 0; j <=int(radius); ++j) {\n"
                    "for (i = 0; i <= int(radius); ++i) {\n"
                        "c = texture(s_TextureMap,uv + vec2(i,j) * src_size).rgb;\n"
                        "m2 += c;\n"
                        "s2 += c * c;\n"
                    "}\n"
                "}\n"
                "for (j = 0; j <=int(radius); ++j) {\n"
                    "for (i = -int(radius); i <= 0; ++i) {\n"
                        "c = texture(s_TextureMap,uv + vec2(i,j) * src_size).rgb;\n"
                        "m3 += c;\n"
                        "s3 += c * c;\n"
                    "}\n"
                "}\n"
                "\n"
                "\n"
                "float min_sigma2 = 1e+2;\n"
                "m0 /= n;\n"
                "s0 = abs(s0 /n - m0 * m0);\n"
                "\n"
                "float sigma2 = s0.r + s0.g + s0.b;\n"
                "if (sigma2 < min_sigma2) {\n"
                    "min_sigma2 = sigma2;\n"
                    "outColor = vec4(m0,1.0);\n"
                "}\n"
                "\n"
                "m1 /= n;\n"
                "s1 = abs(s1 / n -m1 * m1);\n"
                "\n"
                "sigma2 = s1.r + s1.g + s1.b;\n"
                "if (sigma2 < min_sigma2) {\n"
                    "min_sigma2 = sigma2;\n"
                    "outColor = vec4(m1,1.0);\n"
                "}\n"
                "\n"
                "m2 /= n;\n"
                "s2 = abs(s2 / n -m2 * m2);\n"
                "\n"
                "sigma2 = s2.r + s2.g + s2.b;\n"
                "if (sigma2 < min_sigma2) {\n"
                    "min_sigma2 = sigma2;\n"
                    "outColor = vec4(m2,1.0);\n"
                "}\n"
                "\n"
                "m3 /= n;\n"
                "s3 = abs(s3 / n -m3 * m3);\n"
                "\n"
                "sigma2 = s3.r + s3.g + s3.b;\n"
                "if (sigma2 < min_sigma2) {\n"
                    "min_sigma2 = sigma2;\n"
                    "outColor = vec4(m3,1.0);\n"
                "}\n"
        "}\n";

// 旋转
const char swirlFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "in vec2 v_texCoord;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "uniform vec2 u_texSize;\n"
        "uniform highp float radius;\n"
        "uniform highp float angle;\n"
        "uniform vec2 center;\n"
        "void main() {\n"
            "vec2 tc = v_texCoord * u_texSize;\n"
            "tc -= center;\n"
            "float dist = length(tc);\n"
            "if (dist < radius) {\n"
                "float percent = (radius - dist) / radius;\n"
                "float theta = percent * percent * angle * 8.0;\n"
                "float s = sin(theta);\n"
                "float c = cos(theta);\n"
                "tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));\n"
            "}\n"
            "tc += center;\n"
            "outColor = texture(s_TextureMap, tc / u_texSize);\n"
        "}";

// 亮度
const char brightFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "uniform lowp float brightness;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "\n"
        "void main()\n"
        "{\n"
            "lowp vec4 textureColor = texture(s_TextureMap, v_texCoord);\n"
            "\n"
            "outColor = vec4((textureColor.rgb + vec3(brightness)), textureColor.w);\n"
        "}";

// 反转
const char contrastFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "uniform lowp float contrast;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "\n"
        "void main()\n"
        "{\n"
            "lowp vec4 textureColor = texture(s_TextureMap, v_texCoord);\n"
            "\n"
            "outColor = vec4(((textureColor.rgb - vec3(0.5)) * contrast + vec3(0.5)), textureColor.w);\n"
        "}";

// invert
const char invertFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "\n"
        "void main()\n"
        "{\n"
            "lowp vec4 textureColor = texture(s_TextureMap, v_texCoord);\n"
            "\n"
            "outColor = vec4((1.0 - textureColor.rgb), textureColor.w);\n"
        "}";

// pixel
const char pixelFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "uniform float imageWidthFactor;\n"
        "uniform float imageHeightFactor;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "uniform float pixel;\n"
        "layout(location = 0) out vec4 outColor;\n"

        "void main()\n"
        "{\n"
            "vec2 uv = v_texCoord.xy;\n"
            "float dx = pixel * imageWidthFactor;\n"
            "float dy = pixel * imageHeightFactor;\n"
            "vec2 coord = vec2(dx * floor(uv.x / dx), dy * floor(uv.y / dy));\n"
            "vec3 tc = texture(s_TextureMap, coord).xyz;\n"
            "outColor = vec4(tc, 1.0);\n"
        "}";

// vignette
const char vignetteFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "\n"
        "uniform lowp vec2 vignetteCenter;\n"
        "uniform lowp vec3 vignetteColor;\n"
        "uniform highp float vignetteStart;\n"
        "uniform highp float vignetteEnd;\n"
        "\n"
        "void main()\n"
        "{\n"
            "lowp vec3 rgb = texture(s_TextureMap, v_texCoord).rgb;\n"
            "lowp float d = distance(v_texCoord, vec2(0.5,0.5));\n"
            "rgb *= (1.0 - smoothstep(vignetteStart , vignetteEnd, d));\n"
            "outColor = vec4(vec3(rgb), 1.0);\n"
            "\n"
            "lowp vec3 rgb2 = texture(s_TextureMap, v_texCoord).rgb;\n"
            "lowp float d2 = distance(v_texCoord, vec2(vignetteCenter.x, vignetteCenter.y));\n"
            "lowp float percent = smoothstep(vignetteStart, vignetteEnd, d2);\n"
            "outColor = vec4(mix(rgb2.x,vignetteColor.x,percent), mix(rgb2.y, vignetteColor.y, percent), mix(rgb2.z, vignetteColor.z, percent), 1.0);\n"
        "}";

// ColorMatrix
const char colorMatrixFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "\n"
        "uniform lowp mat4 colorMatrix;\n"
        "uniform lowp float intensity;\n"
        "void main()\n"
        "{\n"
            "lowp vec4 textureColor = texture(s_TextureMap, v_texCoord);\n"
            "lowp vec4 outputColor = textureColor * colorMatrix;\n"
            "outColor = (intensity * outputColor) + ((1.0 - intensity) * textureColor);\n"
        "}";

// toon
const char toonFShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "in vec2 leftTextureCoordinate;\n"
        "in vec2 rightTextureCoordinate;\n"
        "in vec2 topTextureCoordinate;\n"
        "in vec2 topLeftTextureCoordinate;\n"
        "in vec2 topRightTextureCoordinate;\n"
        "in vec2 bottomTextureCoordinate;\n"
        "in vec2 bottomLeftTextureCoordinate;\n"
        "in vec2 bottomRightTextureCoordinate;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "uniform highp float intensity;\n"
        "uniform highp float threshold;\n"
        "uniform highp float quantizationLevels;\n"
        "const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);\n"
        "void main()\n"
        "{\n"
            "vec4 textureColor = texture(s_TextureMap, v_texCoord);\n"
            "float bottomLeftIntensity = texture(s_TextureMap, bottomLeftTextureCoordinate).r;\n"
            "float topRightIntensity = texture(s_TextureMap, topRightTextureCoordinate).r;\n"
            "float topLeftIntensity = texture(s_TextureMap, topLeftTextureCoordinate).r;\n"
            "float bottomRightIntensity = texture(s_TextureMap, bottomRightTextureCoordinate).r;\n"
            "float leftIntensity = texture(s_TextureMap, leftTextureCoordinate).r;\n"
            "float rightIntensity = texture(s_TextureMap, rightTextureCoordinate).r;\n"
            "float bottomIntensity = texture(s_TextureMap, bottomTextureCoordinate).r;\n"
            "float topIntensity = texture(s_TextureMap, topTextureCoordinate).r;\n"
            "float h = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity +2.0 * bottomIntensity + bottomRightIntensity;\n"
            "float v = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity +2.0 * rightIntensity + topRightIntensity;\n"
            "float mag = length(vec2(h, v));\n"
            "\n"
            "vec3 posterizedImageColor = floor((textureColor.rgb * quantizationLevels) + 0.5) / quantizationLevels;\n"
            "\n"
            "float thresholdTest = 1.0 - step(threshold, mag);\n"
            "\n"
            "outColor = vec4(posterizedImageColor * thresholdTest, textureColor.a);\n"
        "}";

// grayScale
const char grayScaleShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);\n"
        "void main()\n"
        "{\n"
            "lowp vec4 textureColor = texture(s_TextureMap, v_texCoord);\n"
            "float luminance = dot(textureColor.rgb, W);\n"
            "outColor = vec4(vec3(luminance), textureColor.a);\n"
        "}";

// sketch
const char sketchShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform lowp sampler2D s_TextureMap;\n"

        "in vec2 textureCoordinate;\n"
        "in vec2 leftTextureCoordinate;\n"
        "in vec2 rightTextureCoordinate;\n"
        "\n"
        "in vec2 topTextureCoordinate;\n"
        "in vec2 topLeftTextureCoordinate;\n"
        "in vec2 topRightTextureCoordinate;\n"
        "\n"
        "in vec2 bottomTextureCoordinate;\n"
        "in vec2 bottomLeftTextureCoordinate;\n"
        "in vec2 bottomRightTextureCoordinate;\n"
        "\n"
        "void main()\n"
        "{\n"
            "float bottomLeftIntensity = texture(s_TextureMap, bottomLeftTextureCoordinate).r;\n"
            "float topRightIntensity = texture(s_TextureMap, topRightTextureCoordinate).r;\n"
            "float topLeftIntensity = texture(s_TextureMap, topLeftTextureCoordinate).r;\n"
            "float bottomRightIntensity = texture(s_TextureMap, bottomRightTextureCoordinate).r;\n"
            "float leftIntensity = texture(s_TextureMap, leftTextureCoordinate).r;\n"
            "float rightIntensity = texture(s_TextureMap, rightTextureCoordinate).r;\n"
            "float bottomIntensity = texture(s_TextureMap, bottomTextureCoordinate).r;\n"
            "float topIntensity = texture(s_TextureMap, topTextureCoordinate).r;\n"
            "float h = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity +2.0 * bottomIntensity + bottomRightIntensity;\n"
            "float v = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity +2.0 * rightIntensity + topRightIntensity;\n"
            "float mag = 1.0-length(vec2(h, v));\n"
            "outColor = vec4(vec3(mag), 1.0);\n"
        "}";

// blur
const char blurShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform lowp sampler2D s_TextureMap;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        
        "uniform highp int blurRadius;\n"
        "uniform highp vec2 blurOffset;\n"
        "\n"
        "uniform highp float sumWeight;\n"
        "float PI = 3.1415926;\n"
        "float getWeight(int i)\n"
        "{\n"
            "float sigma = float(blurRadius) / 3.0 ;\n"
            "return (1.0 / sqrt(2.0 * PI * sigma * sigma)) * exp(-float(i * i) / (2.0 * sigma * sigma)) / sumWeight;\n"
        "}\n"

        "vec2 clampCoordinate (vec2 coordinate)\n"
        "{\n"
            "return vec2(clamp(coordinate.x, 0.0, 1.0), clamp(coordinate.y, 0.0, 1.0));\n"
        "}\n"
        "\n"
        "void main()\n"
        "{\n"
            "vec4 sourceColor = texture(s_TextureMap, v_texCoord);\n"
            "if (blurRadius <= 1)\n"
            "{\n"
                "outColor = sourceColor;\n"
                "return;\n"
            "}\n"
            "float weight = getWeight(0);\n"
            "vec3 finalColor = sourceColor.rgb * weight;\n"
            "for(int i = 1; i < blurRadius; i++) {\n"
                "weight = getWeight(i);\n"
                "finalColor += texture(s_TextureMap, clampCoordinate(v_texCoord - blurOffset * float(i))).rgb * weight;\n"
                "finalColor += texture(s_TextureMap, clampCoordinate(v_texCoord + blurOffset * float(i))).rgb * weight;\n"
            "}\n"
            "outColor = vec4(finalColor, sourceColor.a);\n"
        "}\n";

#endif // GPU_ImageETS_constant_shape_H
