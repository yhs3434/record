var obj = {
  vals: [1, 2, 3],
  func: function(v, i) {
    console.log(this, v, i);
  }
};

[4, 5, 6].forEach(obj.func.bind(obj));
