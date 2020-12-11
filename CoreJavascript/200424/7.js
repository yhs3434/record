// forEach, map, filter, reduce 함수에서
// undefined 와 empty 의 결과를 비교.

var arr1 = [undefined, 1];
var arr2 = [];
arr2[1] = 1;

console.log('----forEach----')
arr1.forEach(function (v, i) {console.log(v, i);});
arr2.forEach(function (v, i) {console.log(v, i);});

console.log('----map-----');

console.log(arr1.map(function (v, i) {return v+i;}));
console.log(arr2.map(function (v, i) {return v+i;}));
// undefined + int = NaN
// empty + int = empty

console.log('----filter-----');

var filterArr1 = arr1.filter(function (v) {return !v});
var filterArr2 = arr2.filter(function (v) {return !v});
console.log('case 1: ', filterArr1);
console.log('case 2: ', filterArr2);

console.log('----reduce-----');

var reduceArr1 = arr1.reduce(function (p, c, i) {return p+c+i;}, '');
var reduceArr2 = arr2.reduce(function (p, c, i) {return p+c+i;}, '');

console.log('case 1: ', reduceArr1);
console.log('case 2: ', reduceArr2);

// 결론 : undefined는 문자열로 인식하는 것으로 보임.
// empty는 어떠한 처리도 하지 않음을 볼 수 있음.

/*
  사용자가 명시적으로 부여한 undefined : 인덱스가 존재하며 메모리 내부에 실제로 존재하는 공간
  컴퓨터가 부여한 undefined : 인덱스가 존재하지 않고, 메모리 내부에 실제로는 존재하지 않음.
*/
console.log(typeof null); // 자바스크립트의 버그임. === null 연산자를 사용할 것.
console.log(typeof undefined);
