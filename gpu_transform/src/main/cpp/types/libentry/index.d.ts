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

export const EglRenderInit: () => void;
export const EglRenderSetImageData: (bytes:ArrayBuffer,width:number,height:number) => void;
export const EglRenderSetIntParams: (paramType:number,param:number) => void;
export const EglPixelMapSurface: (x:number,y:number,w:number,h:number) => ArrayBuffer;
export const EglIsInit: () => number;
export const EglUseProgram: () => void;
export const EglRendering: () => void;
export const EglUniform1i: (key:string,value:number) => void;
export const EglUniform1f: (key:string,value:number) => void;
export const EglUniform2fv: (key:string,vf1:number,vf2:number) => void;
export const EglSetTypeArrayOfFloat: (key:string,uniformType:string,data:Float32Array) => void;
export const EglSetTypeArrayOfMatrix3f: (key:string,value:Float32Array) => void;
export const EglSetTypeArrayOfMatrix4f: (key:string,value:Float32Array) => void;
export const EglDestroy: () => void;