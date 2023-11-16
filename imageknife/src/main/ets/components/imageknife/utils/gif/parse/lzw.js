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
 * javascript port of java LZW decompression
 * Original java author url: https://gist.github.com/devunwired/4479231
 */
export function lzw(minCodeSize, data, pixelCount) {
  var MAX_STACK_SIZE = 4096;
  var nullCode = -1;
  var npix = pixelCount;
  var available;
  var clear;
  var codeMask;
  var codeSize;
  var endOfInformation;
  var inCode;
  var oldCode;
  var bits;
  var code;
  var i;
  var datum;
  var dataSize;
  var first;
  var top;
  var bi;
  var pi;
  var dstPixels = new Array(pixelCount);
  var prefix = new Array(MAX_STACK_SIZE);
  var suffix = new Array(MAX_STACK_SIZE);
  var pixelStack = new Array(MAX_STACK_SIZE + 1); // Initialize GIF data stream decoder.

  dataSize = minCodeSize;
  clear = 1 << dataSize;
  endOfInformation = clear + 1;
  available = clear + 2;
  oldCode = nullCode;
  codeSize = dataSize + 1;
  codeMask = (1 << codeSize) - 1;

  for (code = 0; code < clear; code++) {
    prefix[code] = 0;
    suffix[code] = code;
  } // Decode GIF pixel stream.


  var datum, bits, count, first, top, pi, bi;
  datum = bits = count = first = top = pi = bi = 0;

  for (i = 0; i < npix;) {
    if (top === 0) {
      if (bits < codeSize) {
        // get the next byte
        datum += data[bi] << bits;
        bits += 8;
        bi++;
        continue;
      } // Get the next code.


      code = datum & codeMask;
      datum >>= codeSize;
      bits -= codeSize; // Interpret the code

      if (code > available || code == endOfInformation) {
        break;
      }

      if (code == clear) {
        // Reset decoder.
        codeSize = dataSize + 1;
        codeMask = (1 << codeSize) - 1;
        available = clear + 2;
        oldCode = nullCode;
        continue;
      }

      if (oldCode == nullCode) {
        pixelStack[top++] = suffix[code];
        oldCode = code;
        first = code;
        continue;
      }

      inCode = code;

      if (code == available) {
        pixelStack[top++] = first;
        code = oldCode;
      }

      while (code > clear) {
        pixelStack[top++] = suffix[code];
        code = prefix[code];
      }

      first = suffix[code] & 0xff;
      pixelStack[top++] = first; // add a new string to the table, but only if space is available
      // if not, just continue with current table until a clear code is found
      // (deferred clear code implementation as per GIF spec)

      if (available < MAX_STACK_SIZE) {
        prefix[available] = oldCode;
        suffix[available] = first;
        available++;

        if ((available & codeMask) === 0 && available < MAX_STACK_SIZE) {
          codeSize++;
          codeMask += available;
        }
      }

      oldCode = inCode;
    } // Pop a pixel off the pixel stack.


    top--;
    dstPixels[pi++] = pixelStack[top];
    i++;
  }

  for (i = pi; i < npix; i++) {
    dstPixels[i] = 0; // clear missing pixels
  }

  return dstPixels;
};
