"use strict";


// Default stream and parsers for Uint8TypedArray data type
var buildStream = function buildStream(uint8Data) {
  return {
    data: uint8Data,
    pos: 0
  };
};



var readByte = function readByte() {
  return function (stream) {
    return stream.data[stream.pos++];
  };
};



var peekByte = function peekByte() {
  var offset = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 0;
  return function (stream) {
    return stream.data[stream.pos + offset];
  };
};



var readBytes = function readBytes(length) {
  return function (stream) {
    return stream.data.subarray(stream.pos, stream.pos += length);
  };
};


var peekBytes = function peekBytes(length) {
  return function (stream) {
    return stream.data.subarray(stream.pos, stream.pos + length);
  };
};


var readString = function readString(length) {
  return function (stream) {
    return Array.from(readBytes(length)(stream)).map(function (value) {
      return String.fromCharCode(value);
    }).join('');
  };
};



var readUnsigned = function readUnsigned(littleEndian) {
  return function (stream) {
    var bytes = readBytes(2)(stream);
    return littleEndian ? (bytes[1] << 8) + bytes[0] : (bytes[0] << 8) + bytes[1];
  };
};



var readArray = function readArray(byteSize, totalOrFunc) {
  return function (stream, result, parent) {
    var total = typeof totalOrFunc === 'function' ? totalOrFunc(stream, result, parent) : totalOrFunc;
    var parser = readBytes(byteSize);
    var arr = new Array(total);

    for (var i = 0; i < total; i++) {
      arr[i] = parser(stream);
    }

    return arr;
  };
};


var subBitsTotal = function subBitsTotal(bits, startIndex, length) {
  var result = 0;

  for (var i = 0; i < length; i++) {
    result += bits[startIndex + i] && Math.pow(2, length - i - 1);
  }

  return result;
};

var readBits = function readBits(schema) {
  return function (stream) {
    var _byte = readByte()(stream); // convert the byte to bit array


    var bits = new Array(8);

    for (var i = 0; i < 8; i++) {
      bits[7 - i] = !!(_byte & 1 << i);
    } // convert the bit array to values based on the schema


    return Object.keys(schema).reduce(function (res, key) {
      var def = schema[key];

      if (def.length) {
        res[key] = subBitsTotal(bits, def.index, def.length);
      } else {
        res[key] = bits[def.index];
      }

      return res;
    }, {});
  };
};
export {buildStream,readByte,peekByte,readBytes,peekBytes,readString,readUnsigned,readArray,readBits}
