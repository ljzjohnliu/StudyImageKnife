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

/**
 * Deinterlace function from https://github.com/shachaf/jsgif
 */
export function deinterlace(pixels, width) {
  var newPixels = new Array(pixels.length);
  var rows = pixels.length / width;

  var cpRow = function cpRow(toRow, fromRow) {
    var fromPixels = pixels.slice(fromRow * width, (fromRow + 1) * width);
    newPixels.splice.apply(newPixels, [toRow * width, width].concat(fromPixels));
  }; // See appendix E.


  var offsets = [0, 4, 2, 1];
  var steps = [8, 8, 4, 2];
  var fromRow = 0;

  for (var pass = 0; pass < 4; pass++) {
    for (var toRow = offsets[pass]; toRow < rows; toRow += steps[pass]) {
      cpRow(toRow, fromRow);
      fromRow++;
    }
  }
  return newPixels;
};
