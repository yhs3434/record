## 에러 처리

컨트랙트 호출은 중단되고 에러를 반환할 수 있다. 솔리디티에서 에러 제어는 assert, require, revert, throw(현재 더 이상 사용하지 않음)의 네 가지 함수를 사용한다.

에러로 스마트 컨트랙트가 중지될 때는 둘 이상의 컨트랙트가 호출된 경우 컨트랙트 호출 연결을 따라 모든 상태가 원래대로 되돌려진다. 이것은 트랜잭션들이 원자적이라는 것을 보장하는데, 즉 트랜잭션이 성공적으로 완료되거나 실패할 경우 상태에 영향을 미치지 않고 원래대로 완전히 되돌아간다는 것을 의미한다.

assert와 require 함수도 조건을 평가하고, 만약 조건이 거짓이면 에러로 실행을 중지시키는 동일한 방식으로 동작한다. 통상적으로 assert는 결과가 참일 것으로 예상될 때 사용하게 되는데, 이것은 필요한 내적인 조건들이 만족되는지 테스트해 본다는 의미다. 이에 비해 require는 입력값이 설정한 조건의 기댓값에 맞는지 테스트할 때 사용한다.

우리는 메시지 발신자가 컨트랙트의 소유자임을 테스트하기 위해 함수 변경자 onlyOwner에서 require를 사용한다.

``` solidity
require(msg.sender == owner);
```

require 함수는 요구되는 조건이 만족되지 않을 경우 에러를 발생시켜 함수의 나머지 부분이 실행되지 않도록 하는 게이트 조건 기능을 한다.

솔리디티 v0.4.22에서는 에러의 이유를 표시하는 데 사용할 수 있는 유용한 텍스트 메시지를 포함할 수도 있다. 에러 메시지는 트랜잭션 로그로 기록된다. 그래서 우리는 require 함수에 에러 메시지를 추가함으로써 코드를 개선할 수 있다.

``` solidity
require(msg.sender == owner, "only the contract owner can call this function");
```

revert와 throw 함수는 컨트랙트의 실행을 중지하고 모든 변경 상태를 되돌린다. throw 함수는 더는 사용되지 않으며, 향후 버전의 솔리디티에서는 제거될 예정이다. 대신에 revert를 사용해야 한다. revert 함수는 다른 변수 없이 에러 메시지만을 인수로 사용할 수도 있는데, 이 메시지는 트랜잭션 로그에 기록된다.

컨트랙트상의 어떤 조건들은 명시적으로 이를 테스트하도록 설정하지 않더라도 에러를 생성 할 수 있다. 예를 들어, Faucet 컨트랙트에서 출금 요청을 만족시킬 수 있는 이더가 충분한지 여부는 명시적으로 확인하지 않는다. 그 이유는 보낼 이더만큼 잔액이 충분하지 않은 경우 transfer 함수가 에러를 내고 실패할 것이며, 해당 트랜잭션을 되돌릴 것이기 때문이다.

``` solidity
msg.sender.transfer(withdraw_amount);
```

그러나 실패 시 에러 메시지를 분명하게 제공하여 명시적으로 확인하는 것이 좋다. 전송하기 전에 require 문을 추가할 수 있다.

``` solidity
require(this.balance >= withdraw_amount, "Insufficient balance in faucet for withdrawal request");
msg.sender.transfer(withdraw_amount);
```

이와 같은 추가적인 에러 검사 코드는 가스 소비량을 약간 증가시키겠지만, 생략되는 경우보다 나은 에러 리포팅을 얻을 수 있다. 여러분은 컨트랙트에서 예상되는 사용량을 토대로 가스 소비와 자세한 오류 점검 사이에서 적절한 균형을 찾아야 한다. 테스트넷에서 Faucet 컨트랙트의 경우 더 많은 가스 비용이 들더라도 추가적인 보고가 필요할 수도 있지만, 아마도 메인넷 컨트랙트의 경우에는 가스 사용량을 줄이기로 결정하게 될 지도 모른다.

## 이벤트

트랜잭션이 완료되었다면 13장에서 살펴볼 트랜잭션 영수증을 발행한다. 트랜잭션 영수증은 트랜잭션의 실행 동안 발생했던 행위에 관한 정보를 제공하는 로그 엔트리들을 가지고 있다. 이벤트는 이러한 로그를 만들기 위해 사용하는 솔리디티의 고수준 객체다.

이벤트는 특히 라이트 클라이언트와 댑들에 유용한데, 특정한 이벤트가 일어나는지 '감시'해서 사용자 인터페이스에 반영하거나, 해당 컨트랙트상의 이벤트에 대응되는 변화를 애플리케이션의 상태에도 반영되도록 할 수 있기 때문이다.

이벤트 객체는 인수들을 취할 수 있는데, 이것은 시리얼라이즈 되어서 블록체인상의 트랜잭션 로그에 기록된다. 인수 앞에 indexed라는 키워드를 붙여서 애플리케이션에서 검색하거나 필터링할 수 있는, 인덱싱된 테이블의 값으로 만들 수 있다.

지금까지 Faucet 예제에 이벤트를 추가하지 않았으므로 이벤트를 추가해 보자. 두 가지 이벤트를 추가할 것이다. 하나는 출금을 다른 하나는 입금을 로깅할 것이다. 우리는 Withdrawal과 Deposit 이벤트를 각각 호출할 것이다. 먼저 Faucet 컨트랙트에 이벤트를 정의한다.

``` solidity
contract Faucet is mortal {
    event Withdrawal(address indexed to, uint amount);
    event Deposit(address indexed from, uint amount);
    [...]
}
```

Faucet에 엑세스 하는 모든 사용자 인터페이스에서 검색 및 필터링을 할 수 있도록 하기 위해 주소에 indexed를 붙여서 인덱싱되도록 만들었다.

다음은 트랜잭션 로그에 이벤트 데이터를 집어넣기 위해서 emit 키워드를 사용한다.

``` solidity
function withdraw(uint withdraw_amount) public {
    [...]
    msg.sender.transfer(withdraw_amount);
    emit Withdrawal(msg.sender, withdraw_amount);
}

function () public payable {
    emit Deposit(msg.sender, msg.value);
}
```

### 이벤트 받기

이제 우리는 이벤트를 내보내는 컨트랙트를 만들었다. 그렇다면 우리는 어떻게 트랜잭션의 결과를 보고 이벤트를 '받을' 수 있을까? web3.js 라이브러리는 트랜잭션의 로그를 포함하는 데이터 구조를 제공한다. 그 로그를 통해 트랜잭션에 의해 생성된 이벤트를 볼 수 있다.

truffle을 사용하여 수정된 Faucet 컨트랙트에 대한 테스트 트랜잭션을 실행해 보자.

이하 truffle 실습.

이벤트는 컨트랙트 내에서의 커뮤니케이션뿐만 아니라 개발 과정에서의 디버깅에도 매우 유용한 메커니즘이다.

## 다른 컨트랙트 호출(send, call, callcode, delegatecall)

컨트랙트 내에서 다른 컨트랙트를 호출하는 것은 매우 유용하지만, 위험을 내포한 작업이다. 이를 가능토록 하는 다양한 방법을 검토하고 각 방법의 위험을 평가해 본다. 요컨대, 위험은 여러분이 호출하는 컨트랙트 혹은 여러분의 컨트랙트를 호출하는 다른 컨트랙트에 대해 잘 모른다는 사실에서 발생한다. 스마트 컨트랙트를 작성할 때 대부분 EOA만을 다루게 될 것으로 예상하지만, 예상치 못하게 복잡하거나 악의적인 컨트랙트가 여러분의 코드를 호출하거나 또는 반대로 그러한 컨트랙트를 여러분의 코드가 호출할 수도 있음을 염두에 두어야 한다.

### 새로운 인스턴스 만들기

다른 컨트랙트를 호출하기 위한 가장 안전한 방법은 직접 다른 컨트랙트를 만드는 것이다. 그렇게 하면 그것의 인터페이스와 동작이 확실할 수 있다. 이렇게 하기 위해 다른 객체 지향 언어와 마찬가지로 new 키워드를 사용하여 간단하게 인스턴스화할 수 있다. 솔리디티에서 new 키워드는 블록체인에서 컨트랙트를 만들고 이를 참고하는 데 사용할 수 있는 객체를 반환한다. Token이라는 다른 컨트랙트 내에서 Faucet 컨트랙트를 만들고 호출하기를 원한다고 가정해 보자.

``` solidity
contract Token is mortal {
    Faucet _faucet;

    constructor() {
        _faucet = new Faucet();
    }
}
```

이러한 컨트랙트 생성 메커니즘은 해당 컨트랙트의 정확한 타입과 인터페이스를 알 수 있도록 해준다. Faucet 컨트랙트는 Token 영역 안에서 정의되어야 한다. 만약 Faucet 컨트랙트 정의가 다른 파일에 있는 경우 import 구문을 사용하여 Token 영역 안에서 정의되어야 한다.

``` solidity
import "Faucet.sol";

contract Token is mortal {
    Faucet _faucet;

    constructor() {
        _faucet = new Faucet();
    }
}
```

여러분은 인스턴스 생성 시 선택적으로 이더 전송 값(value)을 지정할 수 있고 새 컨트랙트 생성자에게 인수로 전달할 수 있다.

``` solidity
import "Faucet.sol";

contract Token is mortal {
    Faucet _faucet;

    constructor() {
        _faucet = (new Faucet).value(0.5 ether)();
    }
}
```

여러분은 또한 Faucet 함수들을 호출할 수 있다. 예를 들어, Token의 destory 함수 내에서 Faucet의 destroy 함수를 호출할 수 있다.

``` solidity
import "Faucet.sol";

contract Token is mortal {
    Faucet _faucet;

    constructor() {
        _faucet = (new Faucet).value(0.5 ether)();
    }

    function destroy() ownerOnly {
        _faucet.destroy();
    }
}
```

Token 컨트랙트 소유자인 경우 Token 컨트랙트 자체가 새로운 Faucet 컨트랙트를 소유하므로 Token 컨트랙트는 Faucet을 파기할 수 있다.

## 존재하는 인스턴스에 주소 부여하기

다른 컨트랙트를 호출할 수 있는 또 다른 방법은 이미 존재하는 해당 컨트랙트의 인스턴스에 주소를 캐스팅하는 방법이다. 이 방법을 통해 이미 존재하는 인스턴스에 알고 있는 인터페이스를 적용할 수 있다. 그래서 주소를 적용하려는 인스턴스의 유형이 실제 여러분이 가정하고 있는 것인지 확인하는 것이 매우 중요하다.

``` solidity
import "Faucet.sol";

contract Token is mortal {
    Faucet _faucet;

    constructor(address _f) {
        _faucet = Faucet(_f);
        _faucet.withdraw(0.1 ether);
    }
}
```

여기서는 생성자 _f에 대한 인수로 제공된 주소를 가져와서 Faucet 객체로 형변환을 한다. 이는 실제로 주소가 Faucet 객체인지 여부를 확실히 알 수 없기 때문에 이전 메커니즘보다 훨씬 위험하다. withdraw를 호출하면 동일한 인수를 받아들이고 Faucet 선언과 동일한 코드를 실행한다고 가정하지만 확신할 수는 없다. 알다시피 이 주소의 withdraw 함수는 우리가 기대한 것과 전혀 다른 어떤 것을 실행할 수 있다. 따라서 입력으로 전달된 주소를 사용하여 특정 객체에 형변환을 하는 것은 컨트랙트를 직접 작성하는 것보다 훨씬 위험하다.

### 원시 call, delegatecall

솔리디티는 다른 컨트랙트를 호출하기 위한 '저수준'의 함수를 제공한다. 이는 동일한 이름의 EVM 연산코드에 직접적으로 대응하고 컨트랙트 간의 호출을 수동으로 구성할 수 있게 해준다. 따라서 다른 컨트랙트를 호출할 때 가장 유연하면서도 가장 위험한 메커니즘을 나타낸다.

여기에 call 메서드를 사용한 예제가 있다.

``` solidity
contract Token is mortal {
    constructor(address _faucet) {
        _faucet.call("withdraw", 0.1 ether);
    }
}
```

보다시피 이러한 call 형태는 함수 안에 숨은 호출이며, 원시 트랜잭션을 생성하는 것과 매우 비슷하다. 이는 컨트랙트의 컨텍스트에서 발생하고 컨트랙트를 여러 보안 위험에 노출 시킬 수 있는데, 가장 중요한 문제는 재진입성이다. 자세한 내용은 '재진입성'절에서 논의할 것이다. call 함수는 어떤 문제가 있을 경우 false를 반환하기 때문에 에러 처리를 하기 위해서는 그 반환 값을 조사해야 한다.

``` solidity
contract Token is mortal {
    constructor(address _faucet) {
        if !(_faucet.call("withdraw", 0.1 ether)) {
            revert("Withdrawal from faucet failed");
        }
    }
}
```

호출의 또 다른 변형은 delegatecall이다. 이것은 이보다 더 위험한 방식인 callcode를 대체하기 위해 나왔다. callcode 메서드는 곧 사라질 것이므로 사용하지 말아야 한다.

'address 객체' 절에서 언급했듯이, delegatecall은 msg 컨텍스트가 변경되지 않는다는 점에서 call과 다르다. 예를 들어 call은 msg.sender의 값을 호출하는 컨트랙트로 변경하지만, delegatecall은 호출하는 컨트랙트와 동일한 msg.sender를 유지한다. 본질적으로 delegatecall은 현재 컨트랙트의 컨텍스트 내에서 다른 컨트랙트의 코드를 실행한다. 이것은 주로 라이브러리의 코드를 생성할 때 사용한다. 또한, 다른 곳에 저장된 라이브러리 함수를 사용하되 이 함수가 처리해야 되는 데이터는 여러분의 컨트랙트에 저장된 것을 사용하도록 하는 패턴도 가능하게 한다.

delegatecall은 매우 조심해서 사용해야 한다. 특히 여러분이 호출하는 컨트랙트가 라이브러리로 설계되지 않은 경우에는 예상치 못한 결과를 가져올 수 있다.

call과 delegatecall에서 사용되는 다양한 호출 의미를 설명하기 위해 예제 라이브러리를 사용해 보자.

``` solidity
pragma solidity ^0.4.22;

contract calledContract {
    event callEvent(address sender, address origin, address from);
    function calledFunction() public {
        emit callEvent(msg.sender, tx.origin, this);
    }
}

library calledLibrary {
    event callEvent(address sender, address origin, address from);
    function calledFunction() public {
        emit callEvent(msg.sender, tx.origin, this);
    }
}

contract caller {
    function make_calls(calledContract _calledContract) public {
        _calledContract.calledFunction();
        calledLibrary.calledFunction();

        require(address(_calledContract).call(bytes4(keccak256("calledFunction()"))));
        require(address(_callCOntract).delegatecall(bytes4(keccak256("calledFunction()"))));
    }
}
```

예제에서 볼 수 있듯이, 주 컨트랙트는 caller이고 caller는 calledContract 컨트랙트와 calledLibrary 라이브러리를 호출한다. 호출된 컨트랙트와 라이브러리는 calledEvent 이벤트를 발생시키기 위한 동일한 calledFunction 함수를 갖는다. calledEvent는 msg.sender, tx.origin, this 이렇게 3개의 데이터를 로깅한다. calledFunction을 호출할 때마다 직접 호출했는지 아니면 delegatecall을 통해 호출했는지에 따라서 다른 실행 컨텍스트를 가질 수 있다.

caller에서 우선 컨트랙트와 라이브러리를 직접 호출해 보는데, 이 때 각각의 calledFunction을 실행한다. 그런 다음, 저수준 함수인 call과 delegatecall을 명시적으로 사용해서 calledContract.calledFunction을 호출해 보자. 이렇게 함으로써 다양한 호출 메커니즘이 어떻게 작동하는지 확인할 수 있다.

이하 트러플 실습.

## 가스 고려사항

'가스' 절에서 좀 더 자세히 설명할 가스는 스마트 컨트랙트 프로그래밍에서 매우 중요한 고려사항이다. 가스는 이더리움이 트랜잭션이 사용하도록 허용할 최대 계산량을 제한하는 자원이다. 만약 계산하는 동안 가스 한계를 초과하면 다음과 같은 종류의 이벤트가 발생한다.

- '가스 부족' 예외가 발생한다.
- 실행 전의 컨트랙트 상태가 복원된다(복귀).
- 가스를 지급하는 데 사용되는 모든 이더는 트랜잭션 수수료로 간주되고 환불되지 않는다.

가스는 트랜잭션을 시작한 사용자가 지급하기 때문에 사용자는 가스 비용이 높은 함수를 호출하지 않는 것이 좋다. 따라서 프로그래머는 컨트랙트 함수들의 가스 비용을 최소화하도록 해야 한다. 이를 위해 스마트 컨트랙트를 만들 때 함수 호출의 가스 비용을 최소화하기 위해 권장하는 지침들이 있다.

## 동적 크기 배열 피하기

함수가 각 요소에서 연산을 수행하거나 특정 요소를 검색하는 동적 크기 배열을 통한 루프는 너무 많은 가스를 사용하는 위험을 초래한다. 실제로 컨트랙트는 원하는 결과를 찾기 전에 혹은 모든 요소에 적용하기 전에 가스가 소진될지도 모른다. 그러면 아무런 결과 없이 시간과 이더를 낭비하는 셈이다.

## 다른 컨트랙트 호출 피하기

다른 컨트랙트를 호출하는 것은, 특히 그들 함수의 가스 비용이 알려져 있지 않을 때는 가스가 고갈될 위험이 있다. 잘 테스트되지 않고 광범위하게 사용되지 않는 라이브러리는 사용을 피해라. 다른 프로그래머들로부터 받은 점검이 덜 된 라이브러리는 사용상 위험이 더 높다.

## 가스 비용의 추정

컨트랙트의 인수를 고려하여 어떤 컨트랙트 메서드를 실행하는 데 필요한 가스를 추정해야 한다면 다음의 절차를 사용할 수 있다.

``` javascript
var contract = web3.eth.contract(abi).at(address);
var gasEstimate = contract.myAweSomeMethod.estimateGas(arg1, arg2, {from: account});
```

gasEstimate는 실행에 필요한 가스 단위수를 알려준다. 이것은 예상치에 불과한데, 그 이유는 EVM의 튜링 완전성의 특성상 하나의 함수가 여러 가지 다른 호출을 수행하기 위해 소모하는 가스양을 엄청나게 다르게 만드는 게 매우 쉽기 때문이다. 심지어 최종 프로덕션 코드조차도 실행 경로를 미묘하게 변경하여 한 호출에서 다음 호출로의 가스 비용을 크게 변경할 수 있다. 하지만 대부분의 함수는 측정할 수 있고, estimateGas는 대부분의 경우 상당히 정확한 예상치를 제공해 줄 것이다.

네트워크로부터 가스 가격을 얻으려면 다음과 같이 사용할 수 있다.

``` javascript
var gasPrice = web3.eth.getGasPrice();
```

그리고 이것으로부터 가스 비용을 추정할 수 있다.

``` javascript
var gasCostInEther = web3.fromWei((gasEstimate * gasPrice), 'ether');
```

Faucet 예제의 가스 비용을 추정하기 위해 책 저장소의 코드를 사용하여 가스 추정 함수를 적용해 보자. ㅇㅋ

``` javascript
var FaucetContract = artifacts.require("./Faucet.sol");

FaucetContract.web3.eth.getGasPrice(function (error, result) {
    var gasPrice = Number(result);
    
    FaucetContract.deployed().then(function(FaucetContractInstance) {
        FaucetContractInstance.send(web3.toWei(1, "ether"));
        return FaucetContractInstance.withdraw.estimateGas(web3.toWei(0.1, "ether"));
    }).then(function(result) {
        var gas = Number(result);
    });
});
```

트러플 개발 콘소레서 다음과 같이 보일 것이다.

이하 캡처본.