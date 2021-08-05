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

