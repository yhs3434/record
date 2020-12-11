

function closure() {
  var a = 1;
  return function() {
    func1();  // undefined
    //func2();  // func2 is not a function
    function func1 () {
      console.log('func1', a);
    }
    var a = 3;
    var func2 = function () {
      console.log('func2', a);
    }
    func1();  // 3
    func2();  // 3
  }
}
closure()();
