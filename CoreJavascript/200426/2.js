function a () {
  console.log(b);
  var b = 'bbb';
  console.log(b);
  function b () {}
  console.log(b);
}
a();

function a () {
  console.log(c);
  var c = 'ccc';
  console.log(c);
  var c = function () {}
  console.log(c);
}
a();
