var addCoffee = function(name) {
  return new Promise((resolve) => {
    setTimeout(() => {
      resolve(name);
    }, 500)
  })
}

var coffeeMaker = async function() {
  var coffeeList = '';
  var _addCoffee = async function(name) {
    coffeeList += (coffeeList ? ', ' + await addCoffee(name) : await addCoffee(name));
  }
  await _addCoffee('아메리카노');
  console.log(coffeeList);
  await _addCoffee('카페라떼');
  console.log(coffeeList);
  await _addCoffee('아이스티');
  console.log(coffeeList);
};
coffeeMaker();

// 결론 : 제너레이터 기능을 기초적으론 이해했으나, 숙련이 필요할 것으로 보임.
// 제너레이터를 굳이 쓸 일은 없더라도 익숙하게 사용할 수 있도록 하자.
