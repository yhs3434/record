var addCoffee = function (name) {
  return function (prevName) {
    var newName = (prevName ? prevName + ', ' : '') + name;
    return new Promise(function (resolve) {
      setTimeout(function() {
        console.log(newName);
        resolve(newName);
      }, 500)
    })
  }
};

addCoffee('아메리카노')()
.then(addCoffee('카페라떼'))
.then(addCoffee('아이스티'));
