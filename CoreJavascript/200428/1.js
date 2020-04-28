var obj = {
  vals: [1, 2, 3],
  logValues: function(v, i) {
    console.log(this, v, i);
  }
};
obj.logValues(1, 2);

[4,5,6].forEach(obj.logValues);

// 메서드를 콜백으로 전달하면 함수가 됨을 확인할 수 있다.
