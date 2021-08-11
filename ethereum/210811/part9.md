## 예방 기법

스마트 컨트랙트에서 잠재적인 재진입 취약점을 해결하는 데 도움이 되는 일바적인 기술이 여러 가지 있다. 첫 번째는 이더를 외부의 컨트랙트에 보낼 때 내장된 transfer 함수를 사용하는 것이다. transfer 함수는 외부 호출에 대해 2300개의 가스만을 보내는데, 이 정도의 가스양으로 목적지 주소/컨트랙트가 다른 컨트랙트를 호출하기에는 충분하지 않다.

두 번째 방법은 이더가 컨트랙트에서 전송되기 전에 상태 변수를 변경하는 모든 로직이 발생하도록 하는 것이다. 이 EtherStore 예에서 EtherStore.sol의 18행과 19행은 17행 앞에 넣어야 한다. 알 수 없는 주소로 보내는 외부 호출을 수행하는 코드는 지역 함수나 코드 실행 부분에 있어서 가장 마지막 작업이 되도록 하는 것이 바람직하다. 이를 체크 효과 상호작용 패턴이라고 한다.

세 번째 기술은 뮤텍스를 도입하는 것이다. 즉, 코드 실행 중에 컨트랙트를 잠그는 상태 변수를 추가하여 재진입 호출을 방지하는 것이다.

이 모든 기술을 EtherStore.sol에 적용하면 재진입이 일어나지 않는 컨트랙트가 가능하다.

``` solidity
contract EtherStore {
    bool reEntrancyMutex = false;
    uint256 public withdrawalLimit = 1 ether;
    mapping(address => uint256) public lastWithdrawTime;
    mapping(address => uint256) public balances;

    function depositFunds() public payable {
        balances[msg.sender] += msg.value;
    }

    function withdrawFunds(uint256 _weiToWithdraw) public {
        require(!reEntrancyMutex);
        require(balancess[msg.sender] >= _weiToWithdraw);
        // 출금 금액 제한
        require(_weiToWithdraw <= withdrawalLimit);
        // 출금 시간 제한
        require(now >= lastWithdrawTime[msg.sender] + 1 weeks);
        balances[msg.sender] -= _weiToWithdraw;
        lastWithdrawTime[msg.sender] = now;
        // 외부 호출 전에 reEntrancy 뮤텍스 설정
        reEntrancyMutex = true;
        msg.sender.transfer(_weiToWithdraw);
        // 외부 호출 후에 뮤텍스 해제
        reEntrancyMutex = false;
    }
}
```

## 실제 사례 : DAO

DAO(탈중앙화 자율 조직) 공격은 이더리움의 초기 개발에서 발생한 주요 해킹 중 하나였다. 당시 기준으로 컨트랙트는 1억 5000만 달러 이상을 보유하고 있었다. 재진입은 그 공격에서 중요한 역할을 했으며, 궁극적으로는 이더리움 클래식을 만든 하드 포크로 이어졌다

### 산술 오버플로/언더플로

이더리움 가상 머신은 정수들에 대해 고정된 크기의 데이터 타입을 지정한다. 즉, 정수 변수는 특정 범위의 숫자만 나타낼 수 있다. 예를 들어, uint8은 [0, 255] 범위의 숫자만 저장할 수 있다. uint8에 256을 저장하려고 하면 결과는 0이 나온다. 사용자 입력을 점검하지 않고 계산을 수행하면 저장하는 데이터의 유형이 범위를 벗어나는 숫자가 될 수 있어 솔리디티 변수를 악용할 수 있다.

### 취약점

변수의 데이터 타입 범위를 벗어나는 숫자(또는 데이터)를 고정 크기 변수에 저장해야 하는 연산이 수행되면 오버플로/언더플로가 발생한다.

예를 들어, 값이 0인 uint8 변수에서 1을 빼면 결과는 숫자 255가 된다. 이것이 언더플로다. uint8의 범위 아래에 숫자를 할당했으므로 결과가 순환하여 uint8이 저장할 수 있는 가장 큰 숫자를 제공한다. 마찬가지로, uint8에 2^8 = 256을 추가하면 uint의 전체 길이를 순환하게 되어서 변수가 변경되지 않을 것이다. 이런 동작과 유사한 두 가지 사례는 이동 거리를 측정하는 자동차의 주행 거리계와 주기적인 수학 함수다.

데이터 타입 범위보다 큰 숫자를 추가하는 것을 오버플로라고 한다. 명확한 예를 들자면, 현재 0의 값을 갖는 uint8에 257을 추가하면 1이 된다. 고정 크기 변수를 순환하는 것으로 생각하는 편이 때로는 도움이 된다. 즉, 가능한 가장 큰 저장된 숫자보다 큰 숫자를 더하면 0부터 시작하고, 0에서 빼면 가장 큰 숫자에서부터 카운트 다운을 시작한다. 음수를 나타낼 수 있는 부호가 있는 int 유형의 경우 가장 큰 음수 값에 도달하면 다시 시작한다. 예를 들어, 값이 -128인 uint8에서 1을 빼려고 하면 127을 얻을 것이다.

이런 종류의 숫자와 관련된 예기치 못한 문제를 이용하면 공격자가 코드를 악용하여 예기치 않은 논리 흐름을 생성할 수 있다. 예를 들어, 예제 9-3의 TimeLock 컨트랙트를 살펴보라.

``` solidity
contract TimeLock {
    mapping(address => uint) public balances;
    mapping(address => uint) public lockTime;

    function deposit() public payable {
        balances[msg.sender] += msg.value;
        lockTime[msg.sender] = now + 1 weeks;
    }

    function increaseLockTime(uint _secondsToIncrease) public {
        lockTime[msg.sender] += _secondsToIncrease;
    }

    function withdraw() public {
        require(balances[msg.sender] > 0);
        require(now > lockTime[msg.sender]);
        balances[msg.sender] = 0;
        msg.sender.transfer(balance);
    }
}
```

이 컨트랙트는 시간 보관소처럼 작동하도록 설계되었다. 사용자는 컨트랙트에 이더를 보관할 수 있고, 보관된 이더는 최소 1주일 동안 그곳에 잠기게 된다. 사용자는 대기 시간을 일주일 이상 연장할 수 있지만, 일단 입금하면 사용자는 이더가 적어도 일주일 동안 안전하게 잠겨 있다고 확신할 수 있고, 이것이 이 컨트랙트가 이루려는 목적이다.

사용자가 자신의 개인키를 강제적으로 넘겨주어야 하는 상황이 생겼을 경우, 이와 같은 컨트랙트는 자신의 이더가 단기간에 획득될 수 없음을 쉽게 보장할 수 있다. 그러나 사용자의 100이더가 이 컨트랙트에 잠겨 있고 키를 공격자에게 넘겨주면 공격자는 오버플로를 사용하여 lockTime과 무관하게 이더를 빼낼 수 있다.

공격자는 현재 키를 잡고 있는 주소의 현재 lockTime을 결정할 수 있다(공개 변수다). 이것을 userLockTime이라고 부르자. 그런 다음, increaseLockTime 함수를 호출하고 인수로 2^256 - userLockTime을 전달할 수 있다. 이 숫자는 현재 userLockTime에 추가되어 오버플로를 발생시켜 lockTime[msg.sender]를 0으로 재설정한다. 공격자는 간단히 withdraw 함수를 호출하여 보상을 얻을 수 있다.

- 스포일러 경고 : 아직 이더넷 과제를 수행하지 않았다면, 이것은 레벨 중 하나에 대한 해결책을 제시한다.

``` solidity
pragma solidity ^0.4.18;

contract Token {
    mapping(address => uint) balances;
    uint public totalSupply;

    function Token(uint _initialSupply) {
        balances[msg.sender] = totalSupply = _initialSupply;
    }

    function transfer(address _to, uint _value) public returns (bool) {
        require(balances[msg.sender] - _value >= 0);
        balances[msg.sender] -= _value;
        balances[_to] += _value;
        return true;
    }

    function balanceOf(address _owner) public constant returns (uint balance) {
        return balances[_owner];
    }
}
```

이것은 transfer 함수를 사용하는 간단한 토큰 컨트랙트로, 참가자가 토큰을 이동할 수 있게 한다. 이 컨트랙트의 오류를 알 수 있는가?

결함은 transfer 함수에 있다. 언더플로를 사용하면 13행의 require 문을 우회할 수 있다. 잔액이 없는 사용자를 생각해 보라. 그들은 0이 아닌 _value를 가지고 transfer 함수를 호출하고 13행에 있는 require 문을 통과할 수 있다. 이는 balances[msg.sender]가 0이기 때문으로, 양수를 빼면 앞에서 설명했듯이 양수가 된다. 잔액이 양수로 계산되는 14행에서도 마찬가지다. 따라서 이 예에서 공격자는 언더플로 취약점으로 인해 무료 토큰을 얻을 수 있다.

### 예방 기법

언더플로/오버플로 취약점을 방지하기 위한 현재의 일반적인 기술은 표준 수학 연산자인 더하기, 빼기, 곱셈을 대체하는 수학 라이브러리를 사용하거나 만드는 것이다.

오픈제플린은 이더리움 커뮤니티를 위한 보안 라이브러리를 구축하고 감사하는 훌륭한 작업을 수행했다. 특히 SafeMath 라이브러리는 언더플로/오버플로 취약점을 방지하는 데 사용할 수 있다.

솔리디티에서 이런 라이브러리가 사용되는 방법을 보여주기 위해 SafeMath 라이브러리를 사용하여 TimeLock 컨트랙트를 수정해 보자. 오버플로가 일어나지 않는 컨트랙트 버전은 ㅏ음과 같다.

``` solidity
library SafeMath {
    function mul(uint256 a, uint256 b) internal pure returns (uint256) {
        if (a == 0) {
            return 0;
        }
        uint256 c = a * b;
        assert(c / a == b);
        return c;
    }

    function div(uint256 a, uint256 b) internal pure returns (uint256) {
        uint256 c = a / b;
        return c;
    }

    function sub(uint256 a, uint256 b) internal pure returns (uint256) {
        assert(b <= a);
        return a - b;
    }

    function add(uint256 a, uint256 b) internal pure returns (uint256) {
        uint256 c = a + b;
        assert(c >= a);
        return c;
    }
}

contract TimeLock {
    using SafeMath for uint;
    mapping(address => uint256) public balances;
    mapping(address => uint256) public lockTime;

    function deposit() public payable {
        balances[msg.sender] = balances[msg.sender].add(msg.value);
        lockTime[msg.sender] = now.add(1 weeks);
    }

    function increaseLockTime(uint256 _secondsToIncrease) public {
        lockTime[msg.sender] = lockTime[msg.sender].add(_secondsToIncrease);
    }

    function withdraw() public {
        require(balances[msg.sender] > 0);
        require(now > lockTime[msg.sender]);
        balances[msg.sender] = 0;
        msg.sender.transfer(balance);
    }
}
```

모든 표준 수학 연산이 SafeMath 라이브러리에 정의된 연산으로 대체되었다. TimeLock 컨트랙트는 더 이상 언더플로/오버플로가 가능한 어떤 동작도 수행하지 않는다.

### 실제 사례: PoWHC 및 일괄 전송 오버플로

위크 핸즈 증명 코인은 원래 일종의 농담으로 고안되었지만, 한 인터넷 단체에 의해 계획된 폰지 스킴이었다. 불행하게도 계약자는 이전에 오버플로/언더플로를 보지 못했고, 결과적으로 866이더가 컨트랙트에서 유출되었다. 에릭 바니 사드르는 이 사건에 대한 그의 블로그 게시글에서 언더플로가 어떻게 발생했는지 좋은 개요를 제공한다.

또 다른 예는 batchTransfer() 함수를 여러 ERC20 토큰 컨트랙트에 구현한 경우다. 이 구현은 오버플로 취약점이 있었는데, 펙실드의 계정에서 자세한 내용을 읽을 수 있다.

### 예기치 않은 이더

일반적으로 이더가 컨트랙트에 전달될 때는 폴백 함수나 컨트랙트에 정의된 또 다른 함수를 실행해야 한다. 이것에 대해 두가지 예외가 있는데, 어떤 코드를 실행하지 않고 컨트랙트 내에 이더가 존재할 수 있는 경우다. 전송되는 모든 이더에 대해 코드 실행에 의존하는 컨트랙트는 이더가 강제로 전송되는 공격에 취약할 수 있다.

### 취약점

올바른 상태 전이 또는 유효성 검사를 강제하는 데 유용한 일반적인 방어 프로그래밍 기법은 불변 검사다. 이 기법은 하나의 연산 후에 변경되지 않는 불변량을 정의하고 변경되지 않았는지 확인하는 것이다. 검사하는 불변량이 실제로 불변량이라면 이것은 일반적으로 좋은 설계다. 불변량의 한 예는 고정 발행되는 ERC20 토큰의 totalSupply이다. 이 불변부를 수정해야 하는 함수가 없기 때문에 함수가 예상대로 작동하는지 확인하기 위해 totalSupply를 수정하지 않은 상태로 유지하도록 transfer 함수에 체크를 추가할 수 있다.

특히, 불변량으로 사용하고 싶을 정도로 불변량처럼 보이지만, 사실은 외부 사용자들에 의해 조작될 수 있는 것이 있다. 이것은 컨트랙트에 저장된 현재 이더양이다. 종종 개발자가 솔리디티를 배울 때 컨트랙트가 payable 함수를 통해서만 이더를 받아들이거나 얻을 수 있다고 오해를 한다. 이러한 오해로 인해 내부의 이더 잔액에 대한 잘못된 가정을 하는 컨트랙트가 생길 수 있으며, 이로 인해 다양한 취약점이 발생할 수 있다. 이 취약점에 대한 명백한 증거는 this.balance의 사용법이다.

이더가 payable 함수를 사용하거나 컨트랙트에서 코드를 실행하지 않고 컨트랙트에 보내질 수 있는 두 가지 방법이 있다.

- 자기파괴

    모든 컨트랙트는 selfdestruct 함수를 구현할 수 있다. 이 함수는 컨트랙트 주소에서 모든 바이트 코드를 제거하고 거기에 저장된 모든 이더를 파라미터로 지정된 주소로 보낸다. 이 지정된 주소가 컨트랙트인 경우에 어떤 함수도 호출되지 않는다. 이 지정된 주소가 컨트랙트인 경우에 어떤 함수도 호출되지 않는다. 따라서 이 selfdestruct 함수는 컨트랙트에 존재할 수 있는 코드와 관계없이 이더를 강제로 임의의 컨트랙트로 보낼 수 있다. 이것은 아무 공격자나 selfdestruct 함수를 가진 컨트랙트를 만들고, 여기에 이더를 보낸 다음, selfdestruct(target)을 호출해서 target 컨트랙트에 강제로 이더를 보낼 수 있음을 의미한다. 마틴 스웬데는 이에 대한 상세한 블로그 게시물을 작성했다. 거기서 클라이언트 노드가 잘못된 불변량을 검사하는 방법에 대한 설명과 함께 자기파괴 연산코드의 몇 가지 단점을 설명한다. 그리고 그것은 상당히 치명적인 이더리움 네트워크의 붕괴를 가져올 수도 있었다.

- 미리 보내진 이더

    컨트랙트 주소를 미리 알 수 있기 때문에 미리 계산된 주소로 이더를 보낼 수 있다. 컨트랙트 주소는 결정론적이다. 실제로 주소는 컨트랙트를 생성하는 주소와 컨트랙트를 생성하는 트랜잭션 논스의 Keccak-256 해시로 계산된다. 아드리안 매닝의 '키 없는 이더'에 대한 재미있는 사용 사례를 참고하라. 이것은 누구나 컨트랙트가 생성되기 전에 컨트랙트의 주소를 계산할 수 있고, 이 주소로 이더를 보낼 수 있음을 의미한다. 이 컨트랙트가 실제로 생성되었을 때 이것은 0이 아닌 이더 밸런스를 갖게 된다.

이런 사실을 바탕으로 발생할 수 있는 몇 가지 위험을 예제 9-5를 통해 살펴보자.

``` solidity
contract EtherGame {
    uint public payoutMileStone1 = 3 ether;
    uint public mileStone1Reward = 2 ether;
    uint public payoutMileStone2 = 5 ether;
    uint public mileStone2Reward = 3 ether;
    uint public finalMileStone = 10 ether;
    uint public finalReward = 5 ether;

    mapping(address => uint) redeemableEther;
    function play() public payable {
        require(msg.value == 0.5 ether);
        uint currentBalance = this.balance + msg.value;

        require(currentBalance <= finalMileStone);
        
        if (currentBalance == payoutMileStone1) {
            redeemableEther[msg.sender] += mileStone1Reward;
        }
        else if (currentBalance == payoutMileStone2) {
            redeemableEther[msg.sender] += mileStone2Reward;
        }
        else if (currentBalance == finalMileStone) {
            redeemableEther[msg.sender] += finalReward;
        }
    }

    function claimReward() public {
        require(this.balance == finalMileStone);

        require(redeemableEther[msg.sender] > 0);
        redeemableEther[msg.sender] = 0;
        msg.sender.transfer(transferValue);
    }
}
```

이 컨트랙트는 플레이어가 처음으로 세 가지 이정표 중 하나에 먼저 도달하기 위해 0.5이더를 컨트랙트에 보내는 간단한 게임을 나타낸다. 이정표는 이더로 표시된다. 이정표에 도달한 첫 번째 선수는 경기가 끝나면 이더의 일부를 요구할 수 있다. 마지막 이정표에 도달하면 게임이 종료된다. 사용자는 자신의 보상을 청구할 수 있다.

EtherGame 컨트랙트의 문제는 14행, 32행에서 this.balance의 잘못된 사용으로 인한 것이다. 나쁜 공격자는 미래의 어떤 플레이어도 이정표에 도달하지 못하도록 selfdestruct 함수를 사용해 소량의 이더, 예를 들어 0.1이더를 강제로 보낼 수 있다. this.balance는 모든 합법적인 플레이어가 0.5개의 이더 증분만 보낼 수 있기 때문에, 0.1의 이더 기여로 인해 0.5의 배수가 되지는 않는다. 이렇게 하면 18, 21, 24행의 모든 if 조건이 참이 되지 않는다.

더 나쁜 경우로, 이정표를 놓친 복수심에 불타는 공격자는 강제로 10이더를 보내면 컨트랙트의 모든 보상을 영원히 잠글 수 있다. 이것은 claimReward 함수가 32행의 require로 인해 항상 되돌리기 때문이다.

### 예방 기법

이러한 종류의 취약점은 일반적으로 this.balance의 공격으로 인해 발생한다. 컨트랙트 로직은 인위적으로 조작될 수 있기 때문에 가능하다면 컨트랙트 잔액의 정확한 값에 의존하지 않아야 한다. this.balance에 근거한 로직을 적용할 경우 예기치 않은 잔액에 대처해야 한다.

만약 입금된 이더의 정확한 값이 필요하다면, 입금된 이더를 안전하게 추적할 수 있도록 입금 함수 내에서 증가하는 자체 정의된 변수를 사용해야 한다. 이 변수는 selfdestruct 호출을 통해 강제로 보내진 이더에 의해 영향을 받지 않는다.

이를 염두에 두고 수정된 버전의 EtherGame 컨트랙트는 다음과 같다.

``` solidity
contract EtherGame {
    uint public payoutMileStone1 = 3 ether;
    uint public mileStone1Reward = 2 ether;
    uint public payoutMileStone2 = 5 ether;
    uint public mileStone2Reward = 3 ether;
    uint public finalMileStone = 10 ether;
    uint public finalReward = 5 ether;
    uint public depositedWei;

    mapping (address => uint) redeemableEther;

    function play() public payable {
        require(msg.value == 0.5 ether);
        uint currentBalance = depositedWei + msg.value;
        require(currentBalance <= finalMileStone);
        if (currentBalance == payoutMileStone1) {
            redeemableEther[msg.sender] += mileStone1Reward;
        }
        else if (currentBalance == payoutMileStone2) {
            redeemableEther[msg.sender] += mileStone2Reward;
        }
        else if (currentBalance == finalMileStone) {
            redeemableEther[msg.sender] += finalReward;
        }
        depositedWei += msg.value;
        return;
    }

    function claimReward() public {
        require(depositedWei == finalMileStone);
        require(redeemableEther[msg.sender] > 0);
        redeemableEther[msg.sender] = 0;
        msg.sender.transfer(transferValue);
    }
}
```

여기서 우리는 depositedEther라는 새로운 변수를 만들어 입금된 이더를 추적하고 테스트 하는 데 사용했다. 더 이상은 this.balance를 사용하지 않음을 유의하라.

this.balance를 사용하지 않는 이유?
maybe this.balance는 예측 불가능한 입금이 있을 경우가 있으므로, 따로 변수를 선언하여 값을 저장하지 않았을까 하는 생각이 든다.
맞을듯?

### DELEGATECALL

CALL과 DELEGATECALL 연산코드는 이더리움 개발자가 코드를 모듈화할 수 있게 하는 데 유용하다. 컨트랙트에 대한 표준 외부 메시지 호출은 CALL 연산코드에 의해 처리되므로 코드가 외부 컨트랙트/함수의 컨텍스트에서 실행된다. 대상 주소에서 실행 코드가 호출 컨트랙트의 컨텍스트에서 실행되는 것을 제외하고 DELEGATECALL 연산코드는 거의 같으며, msg.sender와 msg.value는 변경되지 않는다. 이 특성을 사용하면 라이브러리를 구현할 수 있으므로 개발자는 재사용 가능한 코드를 일단 배포하고 향후 컨트랙트에서 호출할 수 있다.

이 두 연산코드의 차이점은 간단하고 직관적이지만, DELEGATECALL을 사용하면 예기치 않은 코드가 실행될 수 있다.

### 취약점

DELEGATECALL의 컨텍스트 보존 특성으로 인해 취약점이 없는 사용자 지정 라이브러리를 구축하는 것은 생각만큼 쉽지 않다. 라이브러리 자체의 코드는 안전하고 취약점이 없을 수 있다. 그러나 다른 애플리케이션의 컨텍스트에서 실행될 때 새로운 취약점이 발생할 수 있다. 피보나치 수를 사용하여 이에 대한 상당히 복잡한 예를 살펴보겠다.

``` solidity
contract FibonacciLib {
    uint public start;
    uint public calculatedFibNumber;

    function setStart(uint _start) public {
        start = _start;
    }

    function setFibonacci(uint n) public {
        calculatedFibNumber = fibonacci(n);
    }

    function fibonacci(uint n) internal returns (uint) {
        if (n == 0) return start;
        else if (n == 1) return start + 1;
        else return fibonacci(n - 1) + fibonacci(n - 2);
    }
}
```

이 라이브러리는 수열에서 n번째 피보나치 수를 생성할 수 있는 함수를 제공한다. 사용자는 수열의 시작 번호를 변경하고, 이 새로운 수열에서 n번째 피보나치 같은 숫자를 계산할 수 있다.

이제 이 라이브러리를 사용하는 컨트랙트를 아래 예제에서 살펴보자.

``` solidity
contract FibonacciBalance {
    address public fibonacciLibrary;
    uint public calculatedFibNumber;
    uint public start = 3;
    uint public withdrawalCounter;
    bytes4 constant fibSig = bytes4(sha3("setFibonacci(uint256)"));

    constructor(address _fibonacciLibrary) public payable {
        fibonacciLibrary = _fibonacciLibrary;
    }

    function withdraw() {
        withdrawalCounter += 1;
        require(fibonacciLibrary.delegatecall(fibSig, withdrawalCounter));
        msg.sender.transfer(calculatedFibNumber * 1 ether);
    }

    function() public {
        require(fibonacciLibrary.delegatecall(msg.data));
    }
}
```

이 컨트랙트를 이용해서 참가자는 컨트랙트에서 이더를 출금할 수 있다. 여기서 출금되는 이더의 금액은 참가자의 출금 명령에 상응하는 피보나치 수와 같다. 즉, 첫 번째 참가자는 1이더를 얻고, 두 번째 참가자도 역시 1 이더를 얻고, 세 번째 참가자는 2 이더를 얻고, 네 번째는 3이더를 얻고, 다섯 번째는 5이더가 된다.

이 컨트랙트에는 설명이 필요한 요소들이 있다. 첫 번째로, 흥미로운 변수인 fibSig가 있다. 이것은 'setFibonacci(uint256)' 문자열의 Keccak-256(SHA-3) 해시의 처음 4바이트를 보유한다. 이것은 함수 선택자로 알려져 있으며, 스마트 컨트랙트의 어떤 함수가 호출될지를 지정하기 위해서 calldata에 저장된다. 이것은 21행의 delegatecall 함수에서 fibonacci(uint256) 함수를 실행하고자 한다고 지정하는 데 사용된다. delegatecall의 두 번째 인수는 그 함수에 건네주는 파라미터다. 두 번째로, 우리는 FibonacciLib 라이브러리의 주소가 생성자에서 올바르게 참조되었다고 가정한다.

이 컨트랙트에서 오류를 발견할 수 있는가? 컨트랙트를 배포하고 이더로 채우고 withdraw를 호출하면, 아마 다시 원상태로 되돌아갈 것이다.

상태 변수 start가 라이브러리와 기본 호출 컨트랙트 모두에서 사용된다는 것을 알 수 있다. 라이브러리 컨트랙트에서 start는 피보나치 수열의 시작 부분을 지정하는 데 사용되고 0으로 설정되는 반면, 호출 컨트랙트에서는 3으로 설정된다. 또한 FibonacciBalance 컨트랙트의 폴백 함수로 모든 호출을 라이브러리 컨트랙트에 전달할 수 있으며, 라이브러리 컨트랙트의 setStart 함수를 호출할 수 있다는 것도 알 수 있다. 컨트랙트 상태를 보존한다는 점을 상기하면, 이 함수를 사용하면 지역 변수 FibonnacciBalance 컨트랙트 안에 있는 start 변수의 상태를 변경할 수 있다. 그렇다면 결과로 나온 calculatedFibNumber가 start 변수에 따라 달라지기 때문에 더 많은 이더를 출금할 수 있다. 실제로 setStart 함수는 FibonacciBalance 컨트랙트 안에 있는 start 변수를 수정하지 못한다. 이 컨트랙트의 근본적인 취약점은 start 변수를 수정하는 것보다 훨씬 심각하다.

실제 문제를 논의하기 전에 상태 변수가 실제로 컨트랙트에 저장되는 방법을 살펴보겠다. 상태 또는 스토리지 변수는 컨트랙트에 도입될 때 순차적으로 슬롯에 배치된다.

예를 들어, 라이브러리 컨트랙트를 살펴보겠다. start와 calculatedFibNumber라는 2개의 상태 변수를 갖고 있으며, 첫 번째 변수 start는 컨트랙트의 스토리지 slot[0]에 저장된다. 두 번째 변수 calculatedFibNumber는 다음으로 사용 가능한 스토리지 슬롯에 배치된다. 함수 setStart는 입력 값을 가져와서 start를 입력 값으로 설정한다. 따라서 이 함수는 slot[0]에 setStart 함수의 입력 값을 설정한다. 마찬가지로 setFibonacci 함수는 calculatedFibNumber를 fibonacci(n) 결과로 설정한다. 다시 말하면, 이것은 단순히 스토리지 slot[1]을 fibonacci(n) 값으로 설정하는 것이다.

이제 FibonacciBalance 컨트랙트를 살펴보겠다. 이제 스토리지 slot[0]은 fibonacciLibrary 주소에 해당하고, slot[1]은 calculatedFibNumber에 해당한다. 이 잘못된 매핑에서 취약점이 발생한다. delegatecall은 컨트랙트 컨텍스트를 보존한다. 즉, delegatecall을 통해 실행되는 코드는 호출하는 컨트랙트의 상태에 따라 동작한다.

이제 21행의 withdraw 함수에서 fibonacciLibrary.delegatecall(fibSig, withdrawalCounter)를 실행하는 것에 주목해 보자. 이것은 setFibonacci 함수를 호출하는데, 우리가 논의한 바와 같이 스토리지 slot[1]을 수정한다. 이 슬롯은 현재의 컨텍스트에서 calculatedFibNumber이다. 이것은 예상했던 대로다. 그러나 FibonacciLib 컨트랙트의 start 변수는 현재 컨트랙트의 fibonacciLibrary 주소인 스토리지 slot[0]에 있음을 기억하라. 이것은 fibonacci 함수가 예기치 않은 결과를 줄 수 있음을 의미한다. 이는 현재 호출 컨텍스트에서 fibonacciLibrary 주소인 start를 참고하기 때문이다. 따라서 calculatedFibNumber가 반환할 이더의 uint 양을 포함하지 않기 때문에 withdraw 함수가 원상태로 되돌아가게 된다.

더 나쁜 것은, FibonacciBalance 컨트랙트는 사용자가 26행의 폴백 함수를 통해 모든 fibonacciLibrary 함수를 호출할 수 있도록 허용한다는 점이다. 이전에 논의했듯이, 여기에는 setStart 함수가 포함된다. 우리는 이 함수를 통해 누구나 스토리지 slot[0]을 수정하거나 설정할 수 있다고 설명했다. 이 경우 스토리지 slot[0]은 fibonacciLibrary 주소다. 따라서 공격자는 악의적인 컨트랙트를 만들고, 그 주소를 uint로 변환한 후에 setStart를 호출할 수 있게 된다. 이것은 fibonacciLibrary를 공격 컨트랙트의 주소로 변경한다. 그런 다음, 사용자가 withdraw나 폴백 함수를 호출할 때마다 fibonacciLibrary의 실제 주소를 변경했기 때문에 악의적인 컨트랙트가 실행된다. 이러한 공격 컨트랙트의 예는 다음과 같다.

``` solidity
contract Attack {
    uint storageSlot0;
    uint storageSlot1;

    function() public {
        storageSlot1 = 0;
        <attacker_address>.transfer(this.balance);
    }
}
```

이 공격 컨트랙트는 스토리지 slot[1]을 변경함으로써 calculatedFibNumber를 변경한다. 원칙적으로 공격자는 자신이 선택한 다른 스토리지 슬롯을 수정하여 이 컨트랙트에 대해 모든 종류의 공격을 수행할 수 있다. 우리는 이런 컨트랙트를 리믹스에 넣고 delegatecall 함수를 통해 다양한 공격 컨트랙트와 상태 변경을 시도해 볼 것을 권장한다.

우리가 delegatecall이 상태를 보존한다고 말할 때 그것은 컨트랙트의 변수 이름이 아니라 그 이름이 가리키는 실제 스토리지 슬롯에 대해 말하고 있음을 알아야 한다. 이 예에서 알 수 있듯이, 간단한 실수로 공격자가 전체 컨트랙트와 해당 이더를 가로챌 수 있다.

### 예방 기법

솔리디티는 라이브러리 컨트랙트를 구현하기 위한 키워드를 제공한다. 이것은 라이브러리 컨트랙트가 스테이트리스이며 비자기파괴적임을 보장한다. 라이브러리를 스테이트리스로 만들면 이 절에서 설명하는 스토리지 컨텍스트의 복잡성을 완화할 수 있다. 또한 스테이트리스 라이브러리는 공격자가 라이브러리의 코드를 기반으로 하는 컨트랙트에 영향을 주기 위해 직접 라이브러리의 상태를 수정하는 공격을 방지한다. 일반적으로 DELEGATECALL 라이브러리 컨트랙트와 호출 컨트랙트의 가능한 호출 컨텍스트에 세심하게 신경을 쓰고, 가능한 경우 스테이트리스 라이브러리를 만들어라.

### 실제 사례: 패리티 멀티시그 지갑(두 번째 해킹)

두 번째 패리티 멀티시그 지갑 해킹은 잘 작성된 라이브러리 코드가 의도된 컨텍스트 외부에서 실행될 경우 어떻게 악용될 수 있는지 보여주는 예다. '다시 해킹된 패리티 멀티 시그'와 '패리티 멀티시그 버그에 대해 깊이 살펴보기' 같은 여러가지 좋은 설명이 있다.

이 참고에 추가하기 위해 악용된 컨트랙트를 살펴보겠다. 라이브러리와 지갑 컨트랙트는 깃허브에서 찾을 수 있다.

라이브러리 컨트랙트는 다음과 같다.

``` solidity
contract WalletLibrary is WalletEvents {
    modifier only_uninitialized {
        if (m_numOwners > 0) throw; _;
    }

    function initWallet(address[] _owners, uint _required, uint _daylimit) only_uninitialized {
        initDaylimit(_daylimit);
        initMultiowned(_owners, _required);
    }

    function kill(address _to) onlymanyowners(sha3(msg.data)) external {
        suicide(_to);
    }
}
```

다음은 지갑 컨트랙트다.

``` solidity
contract Wallet is WalletEvents {
    function() payable {
        if (msg.value > 0) {
            Deposit(msg.sender, msg.value);
        }
        else if (msg.data.length > 0) {
            _walletLibrary.delegatecall(msg.data);
        }
    }

    address constant _walletLibrary = 0xcafecafecafe~~~;
}
```

Wallet 컨트랙트는 기본적으로 delegate 호출을 통해 WalletLibrary 컨트랙트에 모든 호출을 전달한다. 이 코드의 상수 _walletLibrary 주소는 실제로 배포된 WalletLibrary 컨트랙트의 자리 표시자 역할을 한다.

이러한 컨트랙트의 목적은 코드 라이브러리와 주요 기능이 WalletLibrary 컨트랙트에 포함된 간단하고 저렴하게 배포 가능한 Wallet 컨트랙트를 만들고자 한 것이었다. 유감스럽게도 WalletLibrary 컨트랙트는 자체가 컨트랙트이며, 그 자체의 상태를 유지한다. 왜 이것이 문제가 될지 이해할 수 있는가?

WalletLibrary 컨트랙트 자체를 호출할 수 있다. 특히 WalletLibrary 컨트랙트를 초기화하고 소유할 수 있다. 실제로 사용자가 WalletLibrary 컨트랙트에서 initWallet 함수를 호출하고 라이브러리 컨트랙트의 소유자가 되었다. 이후 같은 사용자가 kill 함수를 호출했다. 사용자가 라이브러리 컨트랙트의 소유자였기 때문에 모디파이어가 전달되고 라이브러리 컨트랙트가 자기파괴되었다. 모든 Wallet 컨트랙트는 이 라이브러리 컨트랙트를 참고하며, 이 참고를 변경할 방법이 없으므로 WalletLibrary 컨트랙트와 함께 이더 출금 기능을 포함한 모든 기능이 사라져 버렸다. 결과적으로 이 유형의 모든 패리티 멀티시그 지갑의 모든 이더가 즉시 손실되거나 영구적으로 복구할 수 없게 된 것이었다.

## 디폴트 가시성

솔리디티의 함수에는 호출 방법을 지정하는 가시성 지정자가 있다. 가시성은 사용자가 함수를 외부에서 호출할 수 있는지, 다른 파생 컨트랙트가 함수를 내부에서만 또는 외부에서만 호출할 수 있는지 여부를 결정한다. 가시성 지정자는 네 가지가 있으며, 솔리디티 문서에 자세히 설명되어 있다. 함수는 기본적으로 public이며 사용자가 외부에서 호출할 수 있다. 지금부터 가시성 지정자의 잘못된 사용이 스마트 컨트랙트에서 어떤 치명적인 취약점을 초래하는지 알아볼 것이다.

### 취약점

함수에 대한 기본 가시성이 public이므로, 가시성을 지정하지 않은 함수는 외부 사용자가 호출할 수 있다. 개발자가 실수로 private 함수에 대한 가시성 지정자를 생략하면 문제가 발생한다.

``` solidity
contract HashForEther {
    function withdrawWinnings() {
        require(uint32(msg.sender) == 0);
        _sendWinnings();
    }

    function _sendWinnings() {
        msg.sender.transfer(this.balance);
    }
}
```

이 컨트랙트는 사용자가 마지막 8자리 16진수가 0인 이더리움 주소를 생성하면 withdrawWinnings 함수를 호출하여 현상금을 얻을 수 있다.

그러나 불행히도 함수의 가시성은 지정되지 않았다. 특히 _sendWinnings 함수는 public이며, 따라서 어떤 주소에서든 이 함수를 호출하여 현상금을 훔칠 수 있다.

### 예방 기법

함수가 의도적으로 public이라고 할지라도 컨트랙트의 모든 함수에 대한 가시성을 항상 지정하는 것이 좋다. 최근 버전의 solc에서는 명시적인 이러한 방법을 권장하기 위해서 명시적으로 가시성 설정을 하지 않은 함수에는 경고를 표시한다.

### 실제 사례 : 패리티 멀티시그 지갑(첫 번째 해킹)

첫 번째 패리티 멀티시그 해킹에서 약 3,100만 달러 상당의 이더가 도난당했고, 대부분 3개의 지갑에서 발생했다. 이것이 어떻게 행해졌는지에 대한 정리는 하셉 쿠레시가 정리한 글을 참고하자.

본질적으로, 멀티시그 지갑은 기본 지갑 컨트랙트로 구성된다. 이 컨트랙트는 핵심 기능 라이브러리 컨트랙트를 호출한다. 라이브러리 컨트랙트에는 다음 예시 코드에서 볼 수 있듯이 지갑을 초기화하는 코드가 들어있다.

``` solidity
contract WalletLibrary is WalletEvents {
    function initMultiowned(address[] _owners, uint _required) {
        m_numOwners = _owners.length + 1;
        m_owners[1] = uint(msg.sender);
        m_ownerIndex[uint(msg.sender)] = 1;
        for (uint i = 0; i < _owners.length; ++i) {
            m_owners[2 + i] = uint(_owners[i]);
            m_ownerIndex[uint(_owners[i])] = 2 + i;
        }
        m_required = _required;
    }

    function initWallet(address[] _owners, uint _required, uint _daylimit) {
        initDaylimit(_daylimit);
        initMultiowned(_owners, _required);
    }
}
```

