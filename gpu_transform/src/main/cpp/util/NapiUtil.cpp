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

#include "NapiUtil.h"

#include <codecvt>
#include <cstdio>
#include <locale>
#include <string>
#include <string.h>
#include "DebugLog.h"

const int32_t MAX_STR_LENGTH = 1024;

void NapiUtil::JsValueToString(const napi_env &env, const napi_value &value, const int32_t bufLen, std::string &target)
{
    if (bufLen <= 0 || bufLen > MAX_STR_LENGTH) {
        LOGI("%s string too long malloc failed", __func__);
        return;
    }
    
    std::unique_ptr <char[]> buf = std::make_unique <char[]>(bufLen);
    if (buf.get() == nullptr) {
        LOGI("%s nullptr js object to string malloc failed", __func__);
        return;
    }
    (void) memset(buf.get(), 0, bufLen);
    size_t result = 0;
    napi_get_value_string_utf8(env, value, buf.get(), bufLen, &result);
    target = buf.get();
}