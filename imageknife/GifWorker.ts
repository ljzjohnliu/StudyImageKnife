/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import worker, { ThreadWorkerGlobalScope } from '@ohos.worker';
import { parseBufferToFrame } from './src/main/ets/components/imageknife/utils/gif/parse/GIFParse'

export enum LoadType {
    loadBufferByWorker = "loadBufferByWorker"
}

var workerPort : ThreadWorkerGlobalScope = worker.workerPort;

// Send or Receive Format Data Such as: {type: yourResolveType, data: yourDataJson, error?: yourErrorInfo }
export function gifHandler(e) {
    let data = e.data;
    switch (data.type) {
        case LoadType.loadBufferByWorker:
            loadBufferByWorker(data.data, data.type);
            break;
        default:
            break
    }

    function loadBufferByWorker(buffer: ArrayBuffer, recType: string) {
        let images = parseBufferToFrame(buffer);
        let dimss = [];
        let delays = [];
        let disposalTypes = [];
        let patchs = [];
        let transparentIndexs = [];
        for (let i = 0; i < images.length; i++) {
            dimss.push(images[i].dims)
            delays.push(images[i].delay)
            disposalTypes.push(images[i].disposalType)
            patchs.push(images[i].patch.buffer)
            transparentIndexs.push(images[i].transparentIndex)
        }
        let frame = {
            dims: dimss,
            // 当前帧到下一帧的间隔时长
            delay: delays,
            // 当前帧绘制要求 0保留 1在上一帧绘制此帧 2恢复画布背景 3.将画布恢复到绘制当前图像之前的先前状态
            disposalType: disposalTypes,
            // Uint8CampedArray颜色转换后的补片信息用于绘制
            patch: patchs,
            // 表示透明度的可选颜色索引
            transparentIndex: transparentIndexs
        }
        let dataObj = { type: recType, data: frame }
        workerPort.postMessage(dataObj, patchs)
    }
}







