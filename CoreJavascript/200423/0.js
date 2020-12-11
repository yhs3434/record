var Rectangle = function (width, height) {
  this.width = width;
  this.height = height;
};
Rectangle.prototype.getArea = function() {
  return this.width * this.height;
};
var rect = new Rectangle(3, 4);
console.log(rect.getArea());

var Square = function (width) {
  Rectangle.call(this, width, width);
};
Square.prototype = new Rectangle();
var sq = new Square(5);
console.log(sq.getArea());



var rect2 = Rectangle(6, 7);

console.dir(global);

var rect3 = new Rectangle(7, 8);

console.dir(global);
console.dir(this);
console.dir(rect2);
console.dir(rect3);
