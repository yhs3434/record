var coffeeList = '';

var addAmericano = function (name) {
  coffeeList = name;
  console.log(coffeeList);

  setTimeout(addCafelatte, 500, '카페라떼');
}

var addCafelatte = function (name) {
  coffeeList += ', ' + name;
  console.log(coffeeList);

  setTimeout(addIcetea, 500, '아이스 티');
}

var addIcetea = function (name) {
  coffeeList += ', ' + name;
  console.log(coffeeList);
}

setTimeout(addAmericano, 500, '아메리카노');
