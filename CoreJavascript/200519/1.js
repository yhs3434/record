var Rectangle = function (width, height) {
  this.width = width;
  this.height = height;
};

var r1 = new Rectangle(3, 4);
var r2 = new Rectangle(5, 6);

r1.getArea = function() {
  return this.width*this.height;
};

console.log(r1.getArea())
