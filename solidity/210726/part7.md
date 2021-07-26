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

함수와 거래의 실행은 공짜가 아니며 가스비가 든다. 모든 거래에는 가스비가 들고, 호출자는 거래의 성공적인 실행을 위해 가스를 공급할 책임이 있다. 이것은 이더리움의 전역 상태를 수정하는 거래 또는 어떠한 행위에 대해서도 그러하다.

상태 변수를 읽고 반환하기만 하는 함수도 있는데, 이것들은 다른 프로그래밍 언어에서의 게터와 비슷하다. 그것들은 상태 변수에서 현재 값을 읽고 호출자에게 값을 반환한다. 이러한 함수는 이더리움의 상태를 변경하지 않는다. 이더리움 문서에 따르면 다음과 같은 구문은 상태를 변경하는 것으로 본다.

- 상태 변수를 작성
- 이벤트를 생성
- 다른 계약을 생성
- selfdestruct를 사용
- 호출을 통해 이더를 보냄
- view 또는 pure로 표시되지 않은 함수를 호출
- 저수준 호출을 사용
- 특정 연산 코드를 포함하는 인라인 어셈블리를 사용

솔리디티 개발자는 그들의 함수에 view 수정자를 달아서 이 함수가 이더리움의 상태를 변경하거나 앞에서 언급한 행동을 하지 않는다는 것을 EVM에게 알려줄 수 있다. 아직까지는 강제사항이 아니지만 앞으로 그렇게 될 것으로 보인다.

뷰 함수의 예를 다음 예제 코드에서 볼 수 있다.
``` solidity
pragma solidity ^0.4.17;

contract ViewFunction {
    function GetTenerValue(int _data) public view returns (int) {
        return _data * 10;
    }
}
```

어떤 함수가 상태를 변경하지 않고 갑을 반환하기만 한다면 그것을 view 함수로 표시할 수 있다. 참고로 솔리디티의 이전 버전에서는 뷰 함수를 상수(constant) 함수라고 불렀다.

상태 변경 가능성의 관점에서 순수 함수는 뷰 함수에 비해 제약이 많은 편이다. 순수 함수는 뷰 함수와 같은 목적을 갖고 있기는 하지만, 상태 변경 가능성에 대해 좀 더 엄격하다. 이 책을 쓰는 시점에 순수 함수는 강제성이 없지만 미래에는 그렇게 될 것으로 보인다.

순수 함수는 뷰 함수보다 제약이 더 많다. 순수 함수는 이더리움의 현 상태를 읽는 것 조차 허용하지 않는다. 간단히 말해서 순수 함수는 이더리움의 전역 상태를 읽고 쓰는 것을 허용하지 않는다. 문서에 따르면 다음과 같은 추가적인 활동은 허락되지 않는다.

- 상태 변수로부터 읽기
- this.balance 또는 <주소>. balance에 접근
- 블록, 거래, 메시지의 멤버에 접근(msg.sig 및 msg.data는 예외)
- 순수 함수로 표시되지 않은 함수를 호출
- 특정 연산 코드를 포함하는 인라인 어셈블리를 사용

다음 코드는 앞의 함수를 순수 함수로 재작성한 것이다.

``` solidity
pragma solidity ^0.4.17;

contract PureFunction {
    function GetTenerValue(int _data) public pure returns (int) {
        return _data * 10;
    }
}
```


### 주소 함수

자료형을 설명한 장에서는 일부러 주소 자료형에 관련된 함수에 대해 설명하지 않았다. 그중 몇 개는 폴백 함수를 자동적으로 실행할 수 있으므로 여기서 다룬다.

주소와 관련해서 다섯 개의 함수와 한 개의 속성이 있다.

주소의 balance 속성은 계정(계약 계정 또는 개인 계정)의 잔고를 웨이로 나타내며 다음 코드와 같이 사용한다.

<계정>.balance;

여기서 계정은 유효한 이더리움 주소이며, 코드를 실행하면 웨이로 나타낸 잔고를 반환한다. 이제 주소 관련 메소드를 살펴보자.


#### send 메서드

send 메서드는 이더를 계약 또는 개인 소유의 계정에 보내는 데 사용한다. send 메서드는 다음과 같이 사용한다.

<계정>.send(amount);

send는 최대 2,300가스를 제공하며 그것을 초과할 수 없다. 이는 계약 주소에 송금할 때 특히 중요하다. 개인 소유 계정에 송금할 때는 그 만큼의 가스로 충분하다. send는 true 또는 false의 불린 값을 반환한다. 문제없이 실행되면 true를 반환하고, 실패하더라도 false를 반환하며 예외는 발생하지 않는다. send를 계약 주소와 함께 사용하면, 계약에 대해 폴백함수를 호출하게 된다.폴백 함수는 다음 절에서 자세히 살펴본다.

다음 코드에서 send 함수의 예를 살펴본다.

``` solidity
function SimpleSendToAccount() public returns (bool) {
    return msg.sender.send(1);
}
```

위의 예제 코드에서, send 함수는 SimpleSendToAccount의 호출자에게 1웨이를 보낸다. 앞장에서 전역 변수에 대해 다룰 때 msg.sender를 배웠다.

send는 저수준 함수이며, 폴백함수가 호출되어 계약을 재귀적으로 호출하게 될 수 있음에 주의해야 한다. 다음으로 살펴볼 것은 CDF(확인-공제-송금) 또는 CEI((확인-효과-상호작용)이라는 패턴이다. 이 패턴에서는 잔고가 매핑 내에 유지되는 것으로 간주된다. 이 매핑은 주소(address)와 잔고(balance)로 구성된다. 다음 예제 코드를 보자.

``` solidity
mapping (address => uint) balance;

function SimpleSendToAccount(uint amount) public returns (bool) {
    if (balance[msg.sender] >= amount) {
        balance[msg.sender] -= amount;
        if (msg.sender.send(amount) == true) {
            return true;
        }
        else {
            balance[msg.sender] += amount;
            return false;
        }
    }
}
```

이 예에서 호출자가 자금을 인출하기에 충분한 잔고가 있는지 먼저 확인한다. 그러할 경우 잔고에서 금액을 차감하고 send 메서드를 호출한다. 그 다음, send가 성공했는지 반드시 확인해야 한다. 성공하지 못했으면 해당 금액을 돌려준다.

send를 사용하지 말아야 한다고 주장하는 사람도 많지만 저자의 생각은 다르다. 여러 계정에 금액을 보내는 것과 같이 send 함수만의 용도가 여전히 있다. 그렇지만 한 계정에서 다른 계정으로 이더를 보내는 transfer라는 새로운 함수가 도입됐다. 다른 계약과 계정으로 하여금 금액을 인출하는 특정한 메서드를 호출하게 하는 것이 더 나은 방법일 것이다.


#### transfer 메서드

transfer 메서드는 send 메서드와 비슷하며, 특정 주소로 이더 혹은 웨이를 보내는 역할을 한다. transfer는 실행에 실패했을 때 false를 반환하는 대신 예외를 일으키고 모든 변경을 되돌린다는 점이 다르다. 다음 코드의 transfer 메서드를 살펴보자.

``` solidity
function SimpleTransferToAccount() public {
    msg.sender.transfer(1);
}
```

send 메서드에 비해 transfer 메서드가 선호되는데, 오류 이벤트에서 예외를 발생(예외를 스택의 위쪽으로 올리고 수행을 중단)시킨다는 점에서 그러하다.


#### call 메서드

call 메서드는 개발제에게 혼란을 일으킨다. web3.eth 객체를 통해 사용할 수 있는 call 메서드가 있으며, 그와 별개로 <주소>.call 함수도 있다. 두 함수는 서로 목적이 다르다.

web3.eth의 call 메서드는 그것이 연결돼 있는 노드에 대해서만 호출하며 읽기 전용 오퍼레이션이다. 이더리움의 상태를 변경하는 것은 허용되지 않는다. 거래를 발생시키지 않으며 가스를 소비하지도 않는다. 그것은 pure, constant, view 함수를 호출하는 데 사용된다.

한편, 주소 자료형에서 제공하는 call 함수는 계약 내의 어떠한 함수든 호출할 수 있다. 때로는 계약의 인터페이스(ABI)를 사용할 수 없어, call 메서드를 사용하는 것만이 함수를 호출하는 유일한 방법인 경우가 있다. 이 메서드는 ABI를 사용하지 않고 모든 함수를 호출할 수 있다. 이러한 호출에는 컴파일 시 검사가 없으며, true 또는 false의 불린 값을 반환한다.

계약 내의 모든 함수는 4바이트 식별자를 사용해 실행 시간에 식별한다. 이 4바이트 식별자는 함수명과 파라미터 타입을 해시로 줄여 놓은 것이다. 함수명과 파라미터 타입을 해싱한 다음, 첫 4바이트를 함수 식별자로 사용한다. call 함수는 이 바이트를 받아 함수를 호출할 때 첫번째 파라미터로 전달하며, 실제 파라미터 값들은 이후의 파라미터로 전달한다.

다음 코드에서는 call 함수를 호출할 때 SetBalance에 아무런 파라미터를 넣지 않았다.

``` javascript
myaddr.call(bytes4(sha3("SetBalance()")));
```

다음 코드의 call 함수에는 함수 파라미터가 있다. SetBalance는 uint 파라미터 한 개를 취한다.

``` javascript
myaddr.call(bytes4(sha3("SetBalance(uint256)")), 10);
```

앞서 살펴본 send 함수가 실제로는 call 함수를 호출했으며, 이 때 함수에 0가스를 공급했다는 점을 상기해보라.

다음 코드 예에서는 이 함수를 사용할 수 있는 모든 가능한 방법을 보여준다. 이 예에서 EtherBox라는 이름의 계약에는 다음과 같은 두 개의 간단한 함수가 포함된다.

- SetBalance : 한 개의 상태 변수를 가지며, 호출할 때마다 상태 변수에 10을 더한다.
- GetBalance : 상태 변수의 현재 값을 반환하는 함수다.

또 다른 계약 usingCall이 생성되어 call 함수를 통해 EtherBox 계약의 메서드를 호출한다. 다음 코드 예제에서는 다음과 같은 함수들을 살펴보겠다.

1. SimpleCall : 이 함수는 EtherBox 계약의 인스턴스를 생성하고, 그것을 주소로 변환한다. 이 주소를 가지고 call 함수를 사용해 EtherBox 계약의 SetBalance 함수를 호출한다.

2. SimpleCallWithGas : 이 함수는 EtherBox 계약의 인스턴스를 생성하고 그것을 주소로 변환한다. 이 주소를 가지고 call 함수를 사용해 EtherBox의  SetBalance 함수를 호출한다. call과 함께 가스도 보내며, 가스가 더 필요할 경우에 함수 실행이 완료될 수 있다.

3. SimpleCallWithGasAndValue: 이 함수는 EtherBox 계약의 인스턴스를 생성하고 그것을 주소로 변환한다. 이 주소를 가지고 call 함수를 사용해 EtherBox의 SetBalance 함수를 호출한다. call과 함께 가스도 보내며, 가스가 더 필요할 경우에 함수 실행이 완료될 수 있다. 가스와 별개로 이더 혹은 웨이를 payable 함수에 보낼 수도 있다.

다음 코드에서 이러한 함수들을 살펴보자.

``` solidity
pragma solidity ^0.4.17;

contract EtherBox {
    uint balance;

    function SetBalance() public {
        balance = balance + 10;
    }

    function GetBalance() public payable returns (uint) {
        return balance;
    }
}

contract UsingCall {
    function UsingCall() public payable {

    }

    function SimpleCall() public returns (uint) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        status = myaddr.call(bytes4(sha3("SetBalance()")));
        return eb.GetBalance();
    }

    function SimpleCallwithGas() public returns (bool) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        status = myaddr.call.gas(200000)(bytes4(sha3("GetBalance()")));
    }

    function SimpleCallwithGasAndValue() public returns (bool) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        status = myaddr.call.gas(200000).value(1)(bytes4(sha3("GetBalance()")));
        return status;
    }
}
```


#### callcode 메서드

폐기 예정.


#### delegatecall 메서드

이 함수는 호출자의 상태 변수를 사용해 다른 계약 내의 함수를 호출하는 저수준 함수다. 이 함수는 솔리디티의 라이브러리를 통해 사용하는 것이 일반적이다.


### 폴백 함수

폴백 함수는 이더리움에만 있는 특수한 함수다. 솔리디티는 폴백 함수를 작성하도록 돕는다. 독자가 솔리디티 개발자이며, 스마트 계약의 함수를 호출한다고 하자. 계약 내에 존재하지 않는 함수명을 사용하는 경우가 있을 수 있다. 그러한 경우에 폴백 함수가 자동으로 호출된다.

호출된 함수와 이름이 일치하는 함수가 없을 때 폴백 함수가 호출된다.

폴백 함수에는 식별자 또는 함수명이 없다. 이름 없이 정의되는 것이다. 명시적으로 호출할 수 없으므로, 인자도 갖지 않으며 값을 반환하지도 않는다. 다음의 예에서 폴백 함수를 볼 수 있다.

``` solidity
pragma solidity ^0.4.17;

contract FallbackFunction {
    function () {
        var a = 0;
    }
}
```

계약에서 이더를 받을 때도 폴백 함수가 호출될 수 있다. 이것은 종종 한 계정에서 계약으로 이더를 보내기 위해 web3에서 사용 가능한 SendTransaction 함수를 사용해 일어난다. 이 경우 폴백 함수는 payable이어야 하며, 그렇지 않으면 이더를 수취할 수 없고 오류를 발생시킨다.

다음으로 생각해봐야 할 중요한 점은 이 함수를 실행하는데 가스가 얼마나 드는가다. 그것은 명시적으로 호출될 수 없으므로, 이 함수에 가스를 보낼 수는 없다. 그 대신 EVM은 이 함수에 2,300 가스를 제공한다. 이 제한을 초과해서 가스를 소비하면 예외가 발생하며 모든 가스가 원래 함수와 함께 보내진 후 상태가 롤백된다. 그러므로 폴백 함수가 2,300 가스 이상을 소비하지 않는지 테스트하는 것이 중요하다.

또한 스마트 계약에 있어 폴백 함수는 보안상의 허점을 만드는 주요 원인 중 하나임에 유의하자. 계약을 프로덕션에 릴리스하기 전에 이 함수를 테스트 하는 것은 보안 관점에서 매우 중요하다.

몇 가지 예를 통해 폴백 함수를 이해해보자.

주소 자료형의 call 함수를 설명할 때, 사용한 예제를 바탕으로 EtherBox 계약에서 payable 폴백 함수를 구현했다. 이것은 이벤트를 발생시키고 유효하지 않은 함수를 호출한다. 이벤트는 함수 내에서도 선언된다. 다음 장에서 이벤트를 좀 더 깊이 살펴본다.

UsingCall 계약에는 폴백 함수를 호출하는 함수가 없지만 올바른 함수를 호출하지 않는 경우에 폴백 함수가 동작한다. 다음 예제 코드를 보자.

``` solidity
pragma solidity ^0.4.17;

contract EtherBox {
    uint balance;
    event logme(string);

    function SetBalance() public {
        balance = balance + 10;
    }

    function GetBalance() public payable returns (uint) {
        return balance;
    }

    function() payable {
        logme("fallback called");
    }
}

contract UsingCall {
    function UsingCall() public payable {

    }

    function SimpleCall() public returns (uint) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        status = myaddr.call(bytes4(sha3("SetBalance()")));
        return eb.GetBalance();
    }

    function SimpleCallwithGas() public returns (bool) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        status = myaddr.call.gas(200000)(bytes4(sha3("GetBalance()")));
        return status;
    }

    function SimpleCallwithGasAndValue() public returns (bool) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        status = myaddr.call.gas(200000).value(1)(bytes4(sha3("GetBalance()")));
        return status;
    }

    function SimpleCallWithGasAndValueWithWrongName() public returns (bool) {
        bool status = true;
        EtherBox eb = new EtherBox();
        address myaddr = address(eb);
        return myaddr.call.gas(200000).value(1)(bytes4(sha3("GetBalance1()")));
    }
}
```

폴백 함수는 send 메서드, web3의 SendTransaction 함수 또는 transfer 메서드를 사용할 때도 호출된다.