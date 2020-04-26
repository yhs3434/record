function a(x) {
  console.log(x);
  var x;
  console.log(x);
  var x = 2;
  console.log(x);
}
a(1);

console.log('---------');

// 실제로는
function a(x) {
  var x;
  var x;
  var x;

  x = 1;
  console.log(x);
  console.log(x);
  x = 2;
  console.log(x);
}
a(1);
