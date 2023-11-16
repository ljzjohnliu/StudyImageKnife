"use strict";




var parse = function parse(stream, schema) {
  var result = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : {};
  var parent = arguments.length > 3 && arguments[3] !== undefined ? arguments[3] : result;

  if (Array.isArray(schema)) {
    schema.forEach(function (partSchema) {
      return parse(stream, partSchema, result, parent);
    });
  } else if (typeof schema === 'function') {
    schema(stream, result, parent, parse);
  } else {
    var key = Object.keys(schema)[0];

    if (Array.isArray(schema[key])) {
      parent[key] = {};
      parse(stream, schema[key], result, parent[key]);
    } else {
      parent[key] = schema[key](stream, result, parent, parse);
    }
  }

  return result;
};



var conditional = function conditional(schema, conditionFunc) {
  return function (stream, result, parent, parse) {
    if (conditionFunc(stream, result, parent)) {
      parse(stream, schema, result, parent);
    }
  };
};



var loop = function loop(schema, continueFunc) {
  return function (stream, result, parent, parse) {
    var arr = [];
    var lastStreamPos = stream.pos;

    while (continueFunc(stream, result, parent)) {
      var newParent = {};
      parse(stream, schema, result, newParent); // cases when whole file is parsed but no termination is there and stream position is not getting updated as well
      // it falls into infinite recursion, null check to avoid the same

      if (stream.pos === lastStreamPos) {
        break;
      }

      lastStreamPos = stream.pos;
      arr.push(newParent);
    }

    return arr;
  };
};



export{
  loop,
  conditional,
  parse
}

