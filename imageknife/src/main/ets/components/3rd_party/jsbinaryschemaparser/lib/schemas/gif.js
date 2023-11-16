"use strict";


import {
  loop,
  conditional,
  parse
} from "../";

import {buildStream,readByte,peekByte,readBytes,peekBytes,readString,readUnsigned,readArray,readBits}  from "../parsers/uint8";

// a set of 0x00 terminated subblocks
var subBlocksSchema = {
  blocks: function blocks(stream) {
    var terminator = 0x00;
    var chunks = [];
    var streamSize = stream.data.length;
    var total = 0;

    for (var size = (0, readByte)()(stream); size !== terminator; size = (0, readByte)()(stream)) {
      // size becomes undefined for some case when file is corrupted and  terminator is not proper 
      // null check to avoid recursion
      if (!size) break; // catch corrupted files with no terminator

      if (stream.pos + size >= streamSize) {
        var availableSize = streamSize - stream.pos;
        chunks.push((0, readBytes)(availableSize)(stream));
        total += availableSize;
        break;
      }

      chunks.push((0, readBytes)(size)(stream));
      total += size;
    }

    var result = new Uint8Array(total);
    var offset = 0;

    for (var i = 0; i < chunks.length; i++) {
      result.set(chunks[i], offset);
      offset += chunks[i].length;
    }

    return result;
  }
}; // global control extension

var gceSchema = (0, conditional)({
  gce: [{
    codes: (0, readBytes)(2)
  }, {
    byteSize: (0, readByte)()
  }, {
    extras: (0, readBits)({
      future: {
        index: 0,
        length: 3
      },
      disposal: {
        index: 3,
        length: 3
      },
      userInput: {
        index: 6
      },
      transparentColorGiven: {
        index: 7
      }
    })
  }, {
    delay: (0, readUnsigned)(true)
  }, {
    transparentColorIndex: (0, readByte)()
  }, {
    terminator: (0, readByte)()
  }]
}, function (stream) {
  var codes = (0, peekBytes)(2)(stream);
  return codes[0] === 0x21 && codes[1] === 0xf9;
}); // image pipeline block

var imageSchema = (0, conditional)({
  image: [{
    code: (0, readByte)()
  }, {
    descriptor: [{
      left: (0, readUnsigned)(true)
    }, {
      top: (0, readUnsigned)(true)
    }, {
      width: (0, readUnsigned)(true)
    }, {
      height: (0, readUnsigned)(true)
    }, {
      lct: (0, readBits)({
        exists: {
          index: 0
        },
        interlaced: {
          index: 1
        },
        sort: {
          index: 2
        },
        future: {
          index: 3,
          length: 2
        },
        size: {
          index: 5,
          length: 3
        }
      })
    }]
  }, (0, conditional)({
    lct: (0, readArray)(3, function (stream, result, parent) {
      return Math.pow(2, parent.descriptor.lct.size + 1);
    })
  }, function (stream, result, parent) {
    return parent.descriptor.lct.exists;
  }), {
    data: [{
      minCodeSize: (0, readByte)()
    }, subBlocksSchema]
  }]
}, function (stream) {
  return (0, peekByte)()(stream) === 0x2c;
}); // plain text block

var textSchema = (0, conditional)({
  text: [{
    codes: (0, readBytes)(2)
  }, {
    blockSize: (0, readByte)()
  }, {
    preData: function preData(stream, result, parent) {
      return (0, readBytes)(parent.text.blockSize)(stream);
    }
  }, subBlocksSchema]
}, function (stream) {
  var codes = (0, peekBytes)(2)(stream);
  return codes[0] === 0x21 && codes[1] === 0x01;
}); // application block

var applicationSchema = (0, conditional)({
  application: [{
    codes: (0, readBytes)(2)
  }, {
    blockSize: (0, readByte)()
  }, {
    id: function id(stream, result, parent) {
      return (0, readString)(parent.blockSize)(stream);
    }
  }, subBlocksSchema]
}, function (stream) {
  var codes = (0, peekBytes)(2)(stream);
  return codes[0] === 0x21 && codes[1] === 0xff;
}); // comment block

var commentSchema = (0, conditional)({
  comment: [{
    codes: (0, readBytes)(2)
  }, subBlocksSchema]
}, function (stream) {
  var codes = (0, peekBytes)(2)(stream);
  return codes[0] === 0x21 && codes[1] === 0xfe;
});
var schema = [{
  header: [{
    signature: (0, readString)(3)
  }, {
    version: (0, readString)(3)
  }]
}, {
  lsd: [{
    width: (0, readUnsigned)(true)
  }, {
    height: (0, readUnsigned)(true)
  }, {
    gct: (0, readBits)({
      exists: {
        index: 0
      },
      resolution: {
        index: 1,
        length: 3
      },
      sort: {
        index: 4
      },
      size: {
        index: 5,
        length: 3
      }
    })
  }, {
    backgroundColorIndex: (0, readByte)()
  }, {
    pixelAspectRatio: (0, readByte)()
  }]
}, (0, conditional)({
  gct: (0, readArray)(3, function (stream, result) {
    return Math.pow(2, result.lsd.gct.size + 1);
  })
}, function (stream, result) {
  return result.lsd.gct.exists;
}), // content frames
{
  frames: (0, loop)([gceSchema, applicationSchema, commentSchema, imageSchema, textSchema], function (stream) {
    var nextCode = (0, peekByte)()(stream); // rather than check for a terminator, we should check for the existence
    // of an ext or image block to avoid infinite loops
    //var terminator = 0x3B;
    //return nextCode !== terminator;

    return nextCode === 0x21 || nextCode === 0x2c;
  })
}];
var _default = schema;

export {_default}