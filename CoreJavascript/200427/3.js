var func = function () {
  console.log(this, arguments);
};

func(1, 2, 3);
func.call({x: 1}, 1, 2, 3);

var func = (...args) => {
  console.log(this, args);
};

func(1, 2, 3);
func.call({x: 1}, 1, 2, 3);

// arrow function은 call을 써도 this 가 변하지 않음.
// arguments -> args 사용 해야함.
