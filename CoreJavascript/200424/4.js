// 얕은 복사와 깊은 복사

var copyObjectDeep = function (target) {
  var result = {};
  if (target instanceof Object && target !== null) {
    for (var prop in target) {
      result[prop] = copyObjectDeep(target[prop]);
    }
  } else {
    result = target;
  }
  return result;
};

var obj1 = {
  a: 1,
  b: {
    c: null,
    d: [1, 2]
  }
};

var obj2 = copyObjectDeep(obj1);
console.log(obj1);
console.log(obj2);

obj2.b.c = 4;

console.log(obj1 === obj2);
console.log(obj1, obj2);
