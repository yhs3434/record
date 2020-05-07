// 클로저를 활용하여 private한 객체 만들기

var createCar = function () {
  var fuel = Math.ceil(Math.random() * 10 + 10);
  var power = Math.ceil(Math.random() * 3 + 2);
  var moved = 0;
  var returnObj = {
    get moved() {
      return moved;
    },
    run: function () {
      var km = Math.ceil(Math.random() * 6);
      var wasteFuel = km / power;
      if (fuel < wasteFuel) {
        console.log('이동 불가');
        return;
      }
      fuel -= wasteFuel;
      moved += km;
      console.log(km + 'mk 이동 (총 ' + moved + 'mk). 남은 연료: '+fuel);
    }
  }
  Object.freeze(returnObj);
  return returnObj;
}
var car = createCar();
