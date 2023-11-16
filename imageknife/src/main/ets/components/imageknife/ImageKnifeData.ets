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
import { GIFFrame } from './utils/gif/GIFFrame'

export enum ImageKnifeType {
  PIXELMAP = 'PixelMap',
  STRING = 'String',
  RESOURCE = 'Resource',
  GIFFRAME = 'GIFFrame'
}

export class DrawPixelMap {
  imagePixelMap: PixelMap | undefined = undefined
}

export class DrawString {
  imageString: string | undefined = undefined
}

export class DrawResource {
  imageResource: Resource | undefined = undefined
}

export class DrawGIFFrame {
  imageGIFFrames: GIFFrame[] | undefined = undefined
}

export class ImageKnifeData {
  static SVG = 'svg';
  static GIF = 'gif';
  static JPG = 'jpg';
  static PNG = 'png';
  static BMP = 'bmp';
  static WEBP = 'webp';

  waitSaveDisk = false;

  imageKnifeType: ImageKnifeType | undefined = undefined;
  drawPixelMap: DrawPixelMap | undefined = undefined;
  drawGIFFrame: DrawGIFFrame | undefined = undefined;
  drawResource: DrawResource | undefined = undefined;
  drawString: DrawString | undefined = undefined;

  static createImagePixelMap(type: ImageKnifeType, value: PixelMap) {
    let data = new ImageKnifeData();
    data.imageKnifeType = type;
    data.drawPixelMap = new DrawPixelMap();
    data.drawPixelMap.imagePixelMap = value;
    return data;
  }

  static createImageGIFFrame(type: ImageKnifeType, value: GIFFrame[]) {
    let data = new ImageKnifeData();
    data.imageKnifeType = type;
    data.drawGIFFrame = new DrawGIFFrame();
    data.drawGIFFrame.imageGIFFrames = value;
    return data;
  }

  isPixelMap(): boolean {
    return ImageKnifeType.PIXELMAP == this.imageKnifeType;
  }

  isGIFFrame(): boolean {
    return ImageKnifeType.GIFFRAME == this.imageKnifeType;
  }

  isString(): boolean {
    return ImageKnifeType.STRING == this.imageKnifeType;
  }

  isResource(): boolean {
    return ImageKnifeType.RESOURCE == this.imageKnifeType;
  }

  release(){
    if(this.isPixelMap()){
      if(this.drawPixelMap != undefined && this.drawPixelMap.imagePixelMap != undefined){
        this.drawPixelMap.imagePixelMap.release()
          .then(()=>{
            if(this.drawPixelMap != undefined && this.drawPixelMap.imagePixelMap !=undefined){
              this.drawPixelMap.imagePixelMap = undefined;
            }
          })
      }
    }
    if(this.isGIFFrame()){
      if(this.drawGIFFrame != undefined){
        let gifFrames = this.drawGIFFrame.imageGIFFrames;
        if(gifFrames != undefined){
          for (let i = 0; i < gifFrames.length; i++) {
          let tempFrame = gifFrames[i];
          if(tempFrame.drawPixelMap != undefined){
            tempFrame.drawPixelMap.release()
            }
          }
          this.drawGIFFrame.imageGIFFrames = undefined
        }
      }
    }

    if(this.isString()){
      if(this.drawString != undefined && this.drawString.imageString!=undefined){
        this.drawString.imageString = undefined
      }
    }

    if(this.isResource()){
      if(this.drawResource != undefined && this.drawResource.imageResource != undefined){
        this.drawResource.imageResource = undefined
      }
    }

  }

}