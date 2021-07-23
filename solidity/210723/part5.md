### 솔리디티 표현식

하나의 값, 객체, 함수를 반환하는 문장을 표현식이라 한다. 문장은 여러 개의 피연산자와 0개 이상의 연산자로 이뤄진다. 피연산자는 리터럴, 변수, 함수 호출, 그 밖의 표현식 자체일 수 있다.

연산자 + 연산자의 우선순위 설명.

### if 제어 구조

조건문 설명.

### while 루프

while 반복문 설명.


### for 루프

for 반복문 설명.


### do...while 루프

do...while 반복문 설명.

### continue 문

continue 설명


### return 문

데이터 반환.
return 키워드를 사용하지 않고도 함수에서 직접 사용하고 반환 가능.

``` solidity
pragma solidity ^0.4.19;

contract ReturnValues {
    uint counter;

    function SetNumber() {
        counter = block.number;
    }

    function getBlockNumber() returns (uint) {
        return counter;
    }

    function getBlockNumber1() returns (uint result) {
        result = counter;
    }
}
```

