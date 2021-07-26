# 예외, 이벤트, 로깅

### 오류 처리

런타임 오류의 예.

- 가스 부족
- divide by zero
- 자료형 오버플로우
- 배열 인덱스

#### require 문

require라는 단어는 제약이 있음을 의미한다. 다시 말해, require 문을 선언한다는 것은 이후의 코드를 실행하기 전에 충족돼야 할 것을 선언하는 것이다.

``` solidity
pragma solidity ^0.4.19;

contract RequireContract {
    function ValidInt8(uint _data) public returns (uint8) {
        require(_data >= 0);
        require(_data <= 255);

        return uint8(_data);
    }

    function ShouldBeEven(uint _data) public returns (bool) {
        require(_data % 2 == 0);
        return true;
    }
}
```

위의 예제 코드에 나온 함수를 살펴보자.

1. ValidInt8 : 이 함수에는 두 개의 require 문이 있다. 첫 번째 require 문에서는 인자의 값이 0보다 크거나 같은지를 확인한다. 이 문장이 참이면 그 다음 문장을 실행한다. 이 문장이 거짓이면 예외가 발생하고 실행을 중단한다. 두 번째 require 문에서는 인자 값이 255보다 작거나 같은지 확인한다. 인자가 255보다 크면 문장이 거짓으로 평가되어 예외를 던진다.

2. ShouldBeEven : 이 함수도 비슷하다. 여기서는 require 문이 입력 인자가 짝수인지 홀수인지 확인한다. 인자가 짝수이면, 그 다음 문장을 실행하되, 인자가 홀수이면 예외를 던진다.

함수에 들어오는 모든 인자와 값을 require 문을 사용해 검증해야 한다. 즉, 다른 계약의 함수이든 같은 계약의 함수이든 그 입력값은 require 함수를 사용해 확인해야 한다. 변수를 사용하기 전에 require 함수를 가지고 변수의 현재 상태를 확인해야 한다. require가 예외를 던진다면, 전달받은 값은 함수에서 예상하지 못한 값이므로 호출자는 계약에 보내기에 앞서 인자를 수정해야 함을 의미한다.


#### assert 문

assert 문의 구문은 require 문과 비슷하다. 문장을 받으면 그것을 true 또는 false 값으로 평가한다. 그에 따라 다음 문장으로 이동하거나 예외를 던진다. assert는 사용하지 않은 가스를 호출자에게 반환하지 않고, 공급된 가스를 모두 소모한다. 상태는 원래대로 되돌린다. assert 함수가 생성하는 invalid 연산 코드는 상태를 되돌리고 모든 가스를 소모한다.

앞서 예시한 함수를 확장해서 기존 변수에 추가했다. 그렇지만 두 변수를 추가하는 것은 오버플로우 예외를 발생시킬 수 있음을 기억하자. 이를 assert 문을 사용해 검증한다. true를 반환 한다면 값이 반환되고, 그렇지 않으면 예외가 던져진다.

``` solidity
pragma solidity ^0.4.19;

contract AssertContract {
    function ValidInt8(uint _data) public returns (uint8) {
        require(_data >= 0);
        require(_data <= 255);

        uint8 value = 20;

        assert (value + _data <= 255);
        return uint8(_data);
    }
}
```

현재 상태가 일관적이지 않고 실행을 계속하지 말아야 한다고 생각할 때 assert 문을 사용해야 한다.

#### revert 문

revert 문은 require 함수와 매우 비슷하다. 그러나 revert 문은 어떠한 상태도 평가하지 않으며, 어떠한 상태 혹은 구문에도 의존하지 않는다. revert 구문을 사용한다는 것은 예외가 던져지고, 사용되지 않은 가스를 반환하며, 원래 상태로 되돌림을 의미한다.

다음 예에서 if 조건을 사용해 입력 값이 체크될 때 예외가 발생한다. 만약 if 조건 평가가 거짓일 경우 revert 함수가 실행된다. 그 결과로 예외가 발생하고 실행이 정지된다. 다음 예제 코드를 보자.

``` solidity
pragma solidity ^0.4.19;

contract RevertContract {
    function ValidInt8(int _data) public returns (uint8) {
        if (_data < 0 || _data > 255) {
            revert();
        }

        return uint8(_data);
    }
}
```

### 이벤트와 로깅

이벤트는 비동기 애플리케이션을 작성하는 데 도움이 된다. 이벤트는 계약 상속의 일부이며, 자식 계약이 이벤트를 호출할 수 있다. 이벤트 데이터는 블록 데이터와 함께 저장된다.

솔리디티에서 이벤트를 선언하는 방법은 함수를 만드는 것과 유사하다. 그러나 이벤트에는 몸체가 없다. event 키워드와 식별자, 파라미터를 가지고 간단한 이벤트를 선언할 수 있으며, 원하는 파라미터를 이벤트와 함께 보낼 수 있다.

```
event LogFunctionFlow(string);
```

위 코드와 같이 event 라는 키워드 다음에 이벤트의 이름이 오며, 괄호 안에는 이벤트와 함께 보낼 파라미터가 들어간다. LogFunctionFlow 이벤트와 함께 어떤 문자열이든 보낼 수 있다.

이벤트를 사용하는 방법은 상당히 단순하다. 이벤트의 이름을 호출하면서 인자를 전달하면 된다. 다음은 LogFunctionFlow 이벤트를 호출하는 예이며, 파라미터를 가지고 함수를 호출하는 것과 비슷하다.

```
LogFunctionFlow("I am within function x");
```

다음의 코드 조각은 이벤트를 실제로 사용한다. 이 예에서 LogFunctionFlow 이벤트는 문자열을 파라미터로 받도록 선언돼 있다. ValidInt8 함수에서 그 이벤트를 여러 번 호출하는데, 그 때마다 함수의 현재 단계에 대한 정보를 담은 문자열을 제공한다.

``` solidity
pragma solidity ^0.4.19;

contract EventContract {
    event LogFunctionFlow(string);

    function ValidInt8(int _data) public returns (uint8) {
        LogFunctionFlow("Within function ValidInt8");

        if (_data < 0 || _data > 255) {
            revert();
        }

        LogFunctionFlow("Value is within expected range");
        LogFunctionFlow("Returning value from function");

        return uint8(_data);
    }
}
```

이 계약을 리믹스에서 실행한 결과는 다음과 같다. 이벤트 정보가 포함된 로그 항목 3개가 보인다.

이벤트를 감시하는 방법은 두 가지가 있다.

1. 개별 이벤트 감시 : 계약에서 발생한 개별 이벤트를 web3를 사용해 감시 및 추적할 수 있다. 계약에서 이벤트가 발생하면, web3 클라이언트의 함수를 실행시킨다. 다음 코드에서 개별 이벤트를 감시하는 예를 볼 수 있다. 우리가 관심을 갖고 감시할 이벤트의 이름은 ageRead다. 우리는 블록 번호 25000부터 마지막 블록까지 읽는다. 먼저 ageRead 이벤트에 대한 참조가 만들어지고, 감시자가 참조에 추가된다. 감시자는 ageRead 이벤트가 발생할 때마다 실행되는 함수를 갖는다.

``` javascript
var myEvent = instance.ageRead({fromBlock: 25000, toBlock: 'latest'});
myEvent.watch(function(error, result) {
    if (error) {
        console.log(error);
    }
    console.log(result.args);
})
```

2. 전체 이벤트 감시 : 계약의 모든 이벤트를 web3를 사용해 감시 및 추적할 수 있다. 계약에서 어떠한 이벤트가 발생하든지 그것을 통지하고 web3 클라이언트에서 함수의 실행을 돕는다. 이 경우에 이벤트를 이름으로 필터링 할 수 있다. 전체 이벤트를 감시하는 예를 다음 코드에서 볼 수 있다. 여기서 우리는 계약에서 발생하는 임의의 이벤트에 관심을 갖고 감시한다. 우리는 블록 번호 25000부터 마지막 블록까지 읽는다. 먼저 allEvents에 대한 참조가 만들어지며 감시자가 참조에 추가된다. 그런 다음, 감시자는 이벤트가 발생할 때마다 실행되는 함수를 갖는다.

``` javascript
var myEvent = instance.allEvents({fromBlock, toBlock: 'latest'});
myEvent.watch(function(error, result) {
    if (error) {
        console.log(error);
    }
    console.log(result);
})
```