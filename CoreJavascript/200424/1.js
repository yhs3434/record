var ES5 = function (name) {
  this.name = name;
};
ES5.staticMethod = function () {
  return this.name + ' staticMethod';
};
ES5.prototype.method = function () {
  return this.name + ' method';
};
var es5Instance = new ES5('es5');
console.log(ES5.staticMethod());
console.log(es5Instance.method());

var ES6 = class {
  constructor (name) {
    this.name = name;
  }
  static staticMethod () {
    return this.name + ' staticMethod';
  }
  method () {
    return this.name + ' method';
  }
};

var es6Instance = new ES6('es6');
console.log(ES6.staticMethod());
console.log(es6Instance.method());
