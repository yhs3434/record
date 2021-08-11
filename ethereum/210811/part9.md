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
        
        
    }
}