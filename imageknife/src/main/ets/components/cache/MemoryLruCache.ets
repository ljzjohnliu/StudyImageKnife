/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http:// www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import { ImageKnife } from '../imageknife/ImageKnife';
import { ImageKnifeData } from '../imageknife/ImageKnifeData';
import { LruCache } from './LruCache';

export class MemoryLruCache extends LruCache<string, ImageKnifeData>{
  constructor(maxsize:number) {
    super(maxsize)
  }

  // 移除较少使用的缓存数据
  trimToSize(tempsize: number) {
    while (true) {
      if (tempsize < 0) {
        this.map.clear()
        this.size = 0
        break
      }
      if (this.size <= tempsize || this.map.isEmpty()) {
        break
      }
      let delkey = this.map.getFirstKey()
      let data : ImageKnifeData|undefined = this.map.get(delkey)
      if(data != undefined){
        data.release()
      }
      this.map.remove(delkey)
      this.size--
    }
  }

}