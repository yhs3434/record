## javascript event loop

Always. Call stack => event queue.

# hositing

## 정의

hoist 라는 단어의 사전적 정의는 끌어올리기 라는 뜻이다. 자바스크립트에서 끌어올려지는 것은 변수이다. var keyword로 선언된 모든 변수 선언은 호이스트된다. 호이스트란 변수의 정의가 그 범위에 따라 선언과 할당으로 분리되는 것을 의미한다. 즉, 변수가 함수 내에서 정의되었을 경우, 선언이 함수의 최상위로, 함수 바깥에서 정의되었을 경우, 전역 컨텍스트의 최상위로 변경이 된다.

우선, 선언과 할당을 이해해야 한다. 끌어올려지는 것은 선언이다.

``` javascript
function getX() {
    console.log(x);
    var x= 100;
    console.log(x); // 100
}
getX();
```

다른 언어의 경우엔, 변수 x를 선언하지 않고 출력하려 한다면 오류를 발생할 것이다. 하지만 자바스크립트에서는 undefined라고 하고 넘어간다. var x = 100; 이 구문에서 var x;를 호이스트하기 때문이다. 즉, 작동 순서에 맞게 코드를 재구성하면 다음과 같다.

``` javascript
function getX() {
    var x;
    console.log(x);
    x = 100;
    console.log(x);
}
getX();
```

선언문은 항시 자바스크립트 엔진 구동시 가장 최우선으로 해석하므로 호이스팅 되고, 할당 구문은 런타임 과정에서 이루어지기 때문에 호이스팅 되지 않는다.

함수가 자신이 위치한 코드에 상관없이 함수 선언문 형태로 정의한 함수의 유효범위는 전체 코드의 맨 처음부터 시작한다. 함수 선언이 함수 실행 부분보다 뒤에 있더라도 자바스크립트 엔진이 함수 선언을 끌어올리는 것을 의미한다. 함수 호이스팅은 함수를 끌어올리지만 변수의 값은 끌어올리지 않는다.

# 클로저

## 클로저 생성하기

다음은 클로저가 생성되는 조건이다

1. 내부 함수가 익명 함수로 되어 외부 함수의 반환값으로 사용된다.
2. 내부 함수는 외부 함수의 실행 환경에서 실행된다.
3. 내부 함수에서 사용되는 변수 x는 외부 함수의 변수 스코프에 있다.

``` javascript
function outer() {
    var name = 'closure';
    function inner() {
        console.log(name);
    }
    inner();
}
outer();
```

outer함수를 실행시키는 context에는 name이라는 변수가 존재하지 않는다는 것을 확인할 수 있다. 비슷한 맥락에서 코드를 조금 변경해볼 수 있다.

``` javascript
var name = 'warning';
function outer() {
    var name = 'closure';
    return function inner() {
        console.log(name);
    }
}

var callFunc = outer();
callFunc();
// closure
```

위 코드에서 callFunc를 클로저라고 한다. callFunc 호출에 의해 name이라는 값이 console에 찍히는데, 찍히는 값은 Warning이 아니라 closure라는 값이다. 즉, outer 함수의 context에 속해있는 변수를 참조하는 것이다. 여기서 outer 함수의 지역변수로 존재하는 name 변수를 free variable(자유변수) 라고 한다.

이처럼 외부 함수 호출이 종료되더라도 외부 함수의 지역 변수 및 변수 스코프 객체의 체인 관계를 유지할 수 있는 구조를 클로저라고 한다. 보다 정확히는 외부 함수에 의해 반환되는 내부 함수를 가리키는 말이다.

