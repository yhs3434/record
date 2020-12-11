// arrow function 에서의 this binding 확인

var obj = {
  outer : () => {
    console.dir(this);
    var innerFunc = function () {
      console.dir(this);
    }
    innerFunc();
  }
};
obj.outer();

// arrow function은 this를 바인딩 하지 않는다.
// 하지만 그 내부에서 arrow function을 사용하지 않고 선언하면 this는 ES5와 같다.
