var outer = (function() {
  var a = 1;
  var inner = function () {
    return ++a;
  };
  return inner;
})();
console.log(outer());
console.log(outer());
outer = null; // 강제로 메모리 누수를 끊는 방법.
