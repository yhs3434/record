new Promise(function (resolve) {
  setTimeout(function() {
    var name = '아메리카노';
    console.log(name);
    resolve(name);
  }, 500);
}).then(function(name) {
  return new Promise(function (resolve) {
    setTimeout(function() {
      var newName = name + ', ' + '카페라떼';
      console.log(newName);
      resolve(newName);
    }, 500)
  })
}).then(function(name) {
  return new Promise((resolve) => {
    setTimeout(function() {
      var newName = name + ', ' + '아이스티';
      console.log(newName);
    }, 500)
  })
})
