// 깊은 복사 JSON 이용.

var copyObjectViaJSON = function (target) {
  return JSON.parse(JSON.stringify(target));
};

var obj1 = {
  a: 1,
  b: {
    c: null,
    d: [1,2],
    func1: function () {console.log(3)}
  },
  func2: () => {console.log(4)}
};
var obj2 = copyObjectViaJSON(obj1);

obj2.a = 3;
obj2.b.c = 4;

console.log(obj1);
console.log(obj2);

// __proto__, getter/setter, function 은 복사 안됨.
