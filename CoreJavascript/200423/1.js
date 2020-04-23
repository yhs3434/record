var extendClass1 = function (SuperClass, SubClass, methods) {
  SubClass.prototype = new SuperClass();

  for (var prop in SubClass.prototype) {
    if (SubClass.prototype.hasOwnProperty(prop)) {
      delete SubClass.prototype[prop];
    }
  }
  if (methods) {
    for (var method in methods) {
      SubClass.prototype[method] = methods[method];
    }
  }
  Object.freeze(SubClass.prototype);
  return SubClass;
}

var Rectangle = function (width, height) {
  this.width = width;
  this.height = height;
};

var extendClass2 = (function () {
  var Bridge = function () {};
  return function (SuperClass, SubClass, subMethods) {
    Bridge.prototype = SuperClass.prototype;
    SubClass.prototype = new Bridge();
    if (subMethods) {
      for (var method in subMethods) {
        SubClass.prototype[method] = subMethods[method];
      }
    }
    Object.freeze(SubClass.prototype);
    return SubClass;
  }
})();

var Square = extendClass2(Rectangle, function (width) {
  Rectangle.call(this, width, width);
});
