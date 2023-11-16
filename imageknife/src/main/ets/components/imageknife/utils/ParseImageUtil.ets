/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

import {IParseImage} from '../interface/IParseImage'
import image from '@ohos.multimedia.image';
import { BusinessError } from '@ohos.base'
export class ParseImageUtil implements IParseImage<PixelMap> {
  parseImage(imageinfo: ArrayBuffer, onCompleteFunction:(value:PixelMap)=>void | PromiseLike<PixelMap>, onErrorFunction:(reason?:BusinessError|string)=>void) {
    this.parseImageThumbnail(1, imageinfo, onCompleteFunction, onErrorFunction)
  }

  // scale(0,1)
  parseImageThumbnail(scale: number, imageinfo: ArrayBuffer, onCompleteFunction:(value:PixelMap)=>void | PromiseLike<PixelMap>, onErrorFunction:(reason?:BusinessError|string)=>void) {
    let imageSource:image.ImageSource = image.createImageSource(imageinfo); // 步骤一：文件转为pixelMap 然后变换 给Image组件
    imageSource.getImageInfo((err, value) => {
      if (err) {
        onErrorFunction(err);
        return;
      }
      let hValue = Math.round(value.size.height * scale);
      let wValue = Math.round(value.size.width * scale);
      let defaultSize:image.Size = {
        height: hValue,
        width: wValue
      };

      let opts:image.DecodingOptions = {
        editable: true,
        desiredSize: defaultSize
      };


      imageSource.createPixelMap(opts, (err, pixelmap) => {
        if (err) {
          onErrorFunction(err);
        } else {
          onCompleteFunction(pixelmap);
        }
        imageSource.release()
      })

    })
  }
}