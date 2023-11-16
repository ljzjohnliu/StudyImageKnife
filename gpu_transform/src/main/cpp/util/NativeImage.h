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

#ifndef GPU_ImageETS_NativeImage_H
#define GPU_ImageETS_NativeImage_H

#include <stdint.h>
#include <malloc.h>
#include <js_native_api.h>
#include <js_native_api_types.h>
#include <node_api.h>
#include <unistd.h>
#include <string.h>
#include "DebugLog.h"
#include "constant/constant_shape.h"

#define IMAGE_FORMAT_RGBA           0x01
#define IMAGE_FORMAT_NV21           0x02
#define IMAGE_FORMAT_NV12           0x03
#define IMAGE_FORMAT_I420           0x04
#define IMAGE_FORMAT_YUYV           0x05
#define IMAGE_FORMAT_GRAY           0x06
#define IMAGE_FORMAT_I444           0x07
#define IMAGE_FORMAT_P010           0x08

#define IMAGE_FORMAT_RGBA_EXT       "RGB32"
#define IMAGE_FORMAT_NV21_EXT       "NV21"
#define IMAGE_FORMAT_NV12_EXT       "NV12"
#define IMAGE_FORMAT_I420_EXT       "I420"
#define IMAGE_FORMAT_YUYV_EXT       "YUYV"
#define IMAGE_FORMAT_GRAY_EXT       "GRAY"
#define IMAGE_FORMAT_I444_EXT       "I444"
#define IMAGE_FORMAT_P010_EXT       "P010" // 16bit NV21


struct NativeImage {
    int width;
    int height;
    int format;
    uint8_t *ppPlane[DEFAULT_THREE];
    
    NativeImage()
    {
        width = DEFAULT_ZERO;
        height = DEFAULT_ZERO;
        format = DEFAULT_ZERO;
        ppPlane[DEFAULT_ZERO] = nullptr;
        ppPlane[DEFAULT_ONE] = nullptr;
        ppPlane[DEFAULT_TWO] = nullptr;
    }
};

class NativeImageUtil {
public:
        static void AllocNativeImage(NativeImage *pImage)
        {
            if (pImage ->height == DEFAULT_ZERO || pImage ->width == DEFAULT_ZERO) return;
            switch (pImage -> format) {
                case IMAGE_FORMAT_RGBA: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height * DEFAULT_FOUR));
                }
                    break;
                case IMAGE_FORMAT_YUYV: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height * DEFAULT_TWO));
                }
                    break;
                case IMAGE_FORMAT_NV12:
                case IMAGE_FORMAT_NV21: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height * DEFAULT_ONE_HALF));
                    pImage->ppPlane[DEFAULT_ONE] =
                            pImage->ppPlane[DEFAULT_ZERO] + pImage->width * pImage->height;
                }
                    break;
                case IMAGE_FORMAT_I420: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height * DEFAULT_ONE_HALF));
                    pImage->ppPlane[DEFAULT_ONE] =
                            pImage->ppPlane[DEFAULT_ZERO] + pImage->width * pImage->height;
                    pImage->ppPlane[DEFAULT_TWO] =
                            pImage->ppPlane[DEFAULT_ONE] + pImage->width * (pImage->height >> DEFAULT_TWO);
                }
                    break;
                case IMAGE_FORMAT_GRAY: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height));
                }
                    break;
                case IMAGE_FORMAT_I444: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height * DEFAULT_THREE));
                }
                    break;
                case IMAGE_FORMAT_P010: {
                    pImage->ppPlane[DEFAULT_ZERO] =
                            static_cast<uint8_t *>(malloc(pImage->width * pImage ->height * DEFAULT_THREE));
                    pImage->ppPlane[DEFAULT_ONE] =
                            pImage->ppPlane[DEFAULT_ZERO] + pImage->width * pImage->height * DEFAULT_TWO;
                }
                    break;
                default:
                    break;
            }
        }
        
        static void FreeNativeImage(NativeImage *pImage)
        {
            if (pImage == nullptr || pImage->ppPlane[DEFAULT_ZERO] == nullptr) return;
            free(pImage->ppPlane[DEFAULT_ZERO]);
            pImage->ppPlane[DEFAULT_ZERO] = nullptr;
            pImage->ppPlane[DEFAULT_ONE] = nullptr;
            pImage->ppPlane[DEFAULT_TWO] = nullptr;
        }
    
        static bool CreateArrayBuffer(napi_env env, void* src, size_t srcLen, napi_value *res)
        {
            if (src == nullptr || srcLen == DEFAULT_ZERO) {
                return false;
            }
            void *nativePtr = nullptr;
            if (napi_create_arraybuffer(env, srcLen, &nativePtr, res) != napi_ok || nativePtr == nullptr) {
                return false;
            }
            memcpy(nativePtr, src, srcLen);
            return true;
        }
    
        static void Flip(uint8_t** buf, int width, int height)
        {
            int totalLength = width * height * DEFAULT_FOUR;
            int oneLineLength = width * DEFAULT_FOUR;
            uint8_t* tmp = (uint8_t*)malloc(totalLength);
            memcpy(tmp, *buf, totalLength);
            memset(*buf, DEFAULT_ZERO, sizeof(uint8_t)*totalLength);
            for (int i = 0; i < height; i++) {
                memcpy(*buf + oneLineLength * i, tmp + totalLength - oneLineLength * (i+1), oneLineLength);
            }
            free(tmp);
        }
};

#endif // GPU_ImageETS_NativeImage_H
