# 함수, 수정자, 폴백

### 함수 입력과 출력

솔리디티에서는 같은 함수가 여러 파라미터를 받을 수 있다. 단, 그것들의 식별자가 고유한 이름을 가져야 한다.

1. 첫 번째 함수 singleIncomingParameter는 int 형의 파라미터 _data를 받아서 int 형의 _output을 반환한다. 함수 시그니처는 입력 파라미터와 반환 값을 정의하는 구조를 제공한다. 함수 시그니처의 return 키워드는 함수의 반환 타입을 정의하는 것을 돕는다. 다음 코드 조각에서 함수 코드 내의 return 키워드는 함수 시그니처의 첫 번째 반환 타입을 자동적으로 매핑한다.

``` solidity
function singleIncomingParameter(int _data) returns (int _output) {
    return _data * 2;
}
```

2. 두 번째 함수 multipleIncomingParameter는 int 형의 파라미터 _data와 _data2를 받아 int 형의 _output을 반환한다. 다음 코드를 보자.

``` solidity
function multipleIncomingParameter(int _data, int _data2) returns (int _output) {
    return _data * _data2;
}
```

3. 세 번째 함수 multipleOutgoingParameter는 int 형의 파라미터 _data를 받아 int 형의 square와 half를 반환한다. 복수의 파라미터를 반환하는 것은 솔리디티의 고유한 기능으로, 다른 프로그래밍 언어에서는 찾아보기 힘들다.

``` solidity
function multipleOutgoingParameter(int _data) returns (int square, int half) {
    square = _data * _data;
    half = _data / 2;
}
```

4. 네 번째 함수 multipleOutgoingTuple은 세 번째 함수와 비슷하지만, 여러 개의 문장에서 반환값을 할당하는 것이 아니라 튜플을 이용한다는 점이 다르다. 튜플은 여러 개의 변수로 구성된 맞춤 자료구조다. 다음 코드를 보자.

``` solidity
function multipleOutgoingTuple(int _data) returns (int square, int half) {
    (square, half) = (_data * _data, _data / 2);
}
```


계약의 전체 코드는 다음과 같다.

``` solidity
pragma solidity ^0.4.19;

contract Parameters {
    function singleIncomingParameter(int _data) returns (int _output) {
        return _data * 2;
    }

    function multipleIncomingParameter(int _data, int _data2) returns (int _output) {
        return _data * _data2;
    }

    function multipleOutgoingParameter(int _data) returns (int square, int half) {
        square = _data * _data;
        half = _data / 2;
    }

    function multipleOutgoingTuple(int _data) returns (int square, int half) {
        (square, half) = (_data * _data, _data / 2);
    }
}
```


### 수정자

두 코드를 보일 것이다. 첫 번째 코드는 수정자 X, 두 번째 코드는 수정자 O.

``` solidity
pragma solidity ^0.4.17;

contract ContractWithoutModifier {
    address owner;
    int public mydata;

    function ContractWithoutModifier() {
        owner = msg.sender;
    }

    function AssignDoubleValue(int _data) public {
        if (msg.sender == owner) {
            mydata = _data * 2;
        }
    }

    function AssignTenerValue(int _data) public {
        if (msg.sender == owner) {
            mydata = _data * 10;
        }
    }
}
```

수정자는 함수의 행동을 변경하는 특수한 함수다. 함수 코드는 그대로지만 함수의 실행 경로는 바뀐다. 수정자는 함수에만 적용할 수 있다. 같은 계약을 수정자를 사용해 작성한 것을 다음 예제 코드에서 볼 수 있다.

``` solidity
pragma solidity ^0.4.17;

contract ContractWithModifier {
    address owner;
    int public mydata;

    function ContractWithModifier() {
        owner = msg.sender;
    }

    modifier isOwner {
        // require(msg.sender == owner);
        if (msg.sender == owner) {
            _;
        }
    }

    function AssignDoubleValue(int _data) public isOwner {
        mydata = _data * 2;
    }

    function AssignTenerValue(int _data) public isOwner {
        mydata = _data * 10;
    }
}
```

같은 수정자를 여러 함수에 적용할 수 있으며, 호출된 함수 코드를 _ 식별자루 대체할 수 있다. 이는 깔끔하고 가독성 높으며, 유지보수하기 좋은 코드를 작성하는 데 도움이 된다. 개발자는 같은 코드를 모든 함수에 작성하거나 함수를 실행할 때 입력값을 확인하지 않아도 된다.


### 뷰, 상수, 순수 함수

솔리디티에는 함수를 위한 특수한 수정자인 view, pure, constant 등이 있다. 이 것들은 이더리움 전역 상태 내에서 허용된 변화의 스코프를 정의하므로 상태 변경 가능성 속성이라고도 한다. 이러한 수정자들의 목죽은 앞서 논의한 것들과 비슷하지만 약간의 차이가 있다.