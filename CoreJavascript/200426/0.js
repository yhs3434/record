// 호이스팅과 실행 콘텍스트

var a = 1;
function outer() {
  var inner = function() {
    console.log(a);
    var a = 3;
  }
  inner();
  console.log(a);
}
outer();
console.log(a);

console.log('----------');

let b = 1;
function outer() {
  var inner = function() {
    console.log(b);
    b = 3;
  }
  inner();
  console.log(b);
}
outer();
console.log(b);

// let과 var는 실행콘텍스트가 이루어지는 방식이 다른 것 같다.
// => node 에서는 전역객체에 변수가 삽입되지 않는데, 크롬 브라우저에서는 가능함을 알 수 있었다.
