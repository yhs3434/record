### 취약점

함수 내의 지역 변수는 그 타입에 따라 스토리지 또는 메모리를 기본으로 사용한다. 초기화되지 않은 로컬 스토리지 변수에는 컨트랙트의 다른 스토리지 변숫값이 포함될 수 있다. 이러한 사실은 의도하지 않은 취약점을 유발하거나 의도적으로 악용될 수 있다.

``` solidity
contract NameRegistrar {
    bool public unlocked = false;

    struct NameRecord {
        bytes32 name;
        address mappedAddress;
    }

    mapping(address => NameRecord) public registeredNameRecord;
    mapping(bytes32 => address) public resolve;

    function register(bytes32 _name, address _mappedAddress) public {
        NameRecord newRecord;
        newRecord.name = _name;
        newRecord.mappedAddress = _mappedAddress;
        resolve[_name] = _mappedAddress;
        registeredNameRecord[msg.sender] = newRecord;

        require(unlocked);
    }
}
```

이 간단한 이름 등록기는 하나의 기능만 갖고 있다. 컨트랙트의 잠금이 해제되면 누구나 이름을 등록하고 해당 이름을 주소에 매핑할 수 있다. 등록자는 초기에 잠겨 있으며, 25행의 require 함수는 register가 이름 레코드를 추가하지 못하도록 한다. 레지스트리를 잠금 해제할 방법이 없으므로 컨트랙트를 사용할 수 없는 것으로 보인다. 그러나 unlocked 변수에 관계없이 이름 등록을 허용하는 취약점이 있다.

이 취약점에 대해 논의하려면 우선 솔리디티에서 스토리지가 어떻게 작동하는지 이해해야 한다. 개략적으로 상태 변수는 컨트랙트에서 나타나는 대로 슬롯에 순차적으로 저장된다. 따라서 slot[0]에 unlocked, slot[1]에 registeredNameRecord, slot[2]에 resolve 등이 존재한다. 각 슬롯의 크기는 32바이트다. 부울 unlocked는 false의 (주소) 또는 true의 (주소) 처럼 보인다. 보다시피 이 특별한 예에서는 상당한 스토리지 낭비가 있다.

다음 퍼즐 조각은 기본적으로 솔리디티가 struct 같은 복잡한 데이터 타입을 지역 변수로 초기화할 때 스토리지에 저장한다는 것이다. 따라서 18행의 newRecord는 기본적으로 스토리지에 저장된다. 취약점은 newRecord가 초기화되지 않았기 때문에 발생한다. 왜냐하면 newRecord는 기본값이 스토리지이고 그것은 스토리지 slot[0]에 매핑되기 때문이다. 19행과 20행에서 newRecord.name을 _name으로 설정하고 newRecord.mappedAddress를 _mappedAddress로 설정한다. 이것은 slot[0] 및 slot[1]의 저장 위치를 갱신하며, 이는 unlocked와 registeredNameRecord 관련 저장 슬롯을 모두 수정한다.

이는 unlocked는 register 함수의 bytes32 _name 파라미터에 의해 직접 수정될 수 있음을 의미한다. 따라서 _name의 마지막 바이트가 0이 아닌 경우 스토리지 slot[0]의 마지막 바이트를 수정하고 unlocked를 true로 직접 변경한다. 이러한 _name 값은 unlocked를 true로 설정했기 때문에 25행에서 require 호출이 성공하도록 한다. 리믹스에서 시도해 보면, 거기서 _name을 사용하면 함수가 통과할 것이다.

### 예방 기법

솔리디티 컴파일러는 초기화되지 않은 스토리지 변수에 대한 경고를 보여준다. 개발자는 스마트 컨트랙트를 작성할 때 이러한 경고에 주의를 기울여야 한다. 미스트의 현재 버전에서는 이러한 컨트랙트를 컴파일할 수 없다. 복잡한 유형을 처리할 때 memory 또는 storage 지정자를 명시적으로 사용하여 예상대로 작동하는지 확인하는 것이 좋다.

### 실제 사례 : OpenAddressLottery와 CryptoRoulette 허니팟

OpenAddressLottery라는 이름의 허니팟이 배포되었는데, 해커라고 자랑하고픈 사람들로부터 이더를 뺏어오기 위해 이 초기화되지 않은 스토리지 변수 쿼크를 사용했다. 이 컨트랙트는 상당히 뒤얽혀 있어서 이에 대한 분석은 공격을 상당히 깔끔하게 설명하고 있는 레딧 스레드에 넘기도록 하겠다.

또 다른 허니팟도 이 트릭을 사용해 일부 이더를 수집하려고 한다. 공격의 작동 방식을 파악할 수 없는 경우 이 컨트랙트 및 다른 내용에 대한 개요는 '한 쌍의 이더리움 허니팟 컨트랙트 분석'을 참고하라.

### 부동소수점 및 정밀도

이 글을 쓰는 시점의 솔리디티는 고정소수점 및 부동 소수점 숫자를 지원하지 않는다. 즉, 부동소수점 표현은 솔리디티에서 정수 유형으로 구성해야 한다. 이로 인해 올바르게 구현되지 않으면 오류 및 취약점이 발생할 수 있다.

### 취약점

솔리디티에는 고정소수점 유형이 없기 때문에 개발자는 표준 정수 데이터 타입을 사용하여 자체적으로 구현해야 한다. 이 과정에서 개발자가 범할 수 있는 여러 가지 함정이 있다. 이 절에서는 이 중 일부를 강조할 것이다.

``` solidity
contract FunWithNumbers {
    uint constant public tokensPerEth = 10;
    uint constant public weiPerEth = 1e18;
    mapping(address => uint) public balances;

    function buyTokens() public payable {
        uint tokens = msg.value / weiPerEth * tokensPerEth;
        balances[msg.sender] += tokens;
    }

    function sellTokens(uint tokens) public {
        require(balances[msg.sender] >= tokens);
        uint eth = tokens/tokensPerEth;
        balances[msg.sender] -= tokens;
        msg.sender.transfer(eth * weiPerEth);
    }
}
```

이 간단한 토큰 매매 컨트랙트에는 명백한 문제가 있다. 토큰 매매에 대한 수학적 계산은 정확하지만, 부동소수점 숫자는 잘못된 결과를 준다. 예를 들어, 8행의 토큰을 구매할 때 값이 1보다 작은 경우 초기 나누기는 결과가 0이 되고 최종 곱하기의 결과는 0이 된다. 마찬가지로 토큰을 판매할 때 10개 미만의 토큰이 있으면 결과적으로 0이더가 된다. 실제로 여기서 반올림하는 것은 항상 버림되므로 29개의 토큰을 팔면 2개의 이더가 된다.

이 컨트랙트의 이슈는 정밀도가 단지 이더 근삿값에만 해당하는 것이다. 더 높은 정밀도가 필요한 ERC20 토큰의 십진수를 다룰 때 까다로울 수 있다.

### 예방 기법

스마트 컨트랙트에서 올바른 정확성을 유지하는 것은 매우 중요한데, 특히 경제적인 결정을 반영하는 비율을 다룰 때 매우 중요하다.

비나 비율을 사용할 때는 분수에서 큰 분자를 사용할 수 있는지 확인해야 한다. 예를 들어, 위의 예에서는 tokensPerEth 비율을 사용했다. 큰 수인 weiPerTokens를 사용하는 것이 더 좋았을 것이다. 대응하는 토큰 수를 계산하려면 msg.sender/weiPerTokens를 사용할 수 있다. 이렇게 하면 좀 더 정확한 결과를 얻을 수 있다.

생각해 둬야 할 또 다른 전술은 작업 순서를 염두에 두는 것이다. 이 예에서 토큰을 구입하는 계산은 msg.value/weiPerEth * tokenPerEth이다. 나누기는 곱하기 전에 발생한다. 이 예에서는 먼저 곱셈을 수행한 후에 나누기를 수행하면 더 높은 정밀도를 얻었을지도 모른다.

마지막으로, 숫자에 대해 임의의 정밀도를 정의할 때는 값을 더 높은 정밀도로 변환하고 모든 수학 연산을 수행한 다음, 최종적으로 출력에 필요한 정밀도로 다시 변환하는 것이 좋다. 일반적으로 uint256이 사용된다. 이것들은 그 범위에서 약 60배의 크기를 제공하며, 그중 일부는 수학적 연산의 정밀도에 전담시킬 수 있다. 솔리디티에서는 모든 변수를 높은 정밀도로 유지하고 외부 앱의 낮은 정밀도로 다시 변환하는 것이 더 나은 경우일 수 있다. 파격적인 호칭을 사용하긴 하지만 개념은 유용하다.

### 실제 사례 : 에스틱

에스틱 컨트랙트는 확장된 정밀도를 사용하지는 않지만, wei를 다룬다. 따라서 이 컨트랙트에는 반올림 문제가 있지만, wei 수준의 정밀도에서만 문제가 된다. 여기에는 더 심각한 결함들이 있기는 한데, 이것은 블록체인에서 엔트로피를 얻는 어려움과 관련이 있다. 에스틱 컨트랙트에 대한 자세한 내용은 피터 베세네스의 이더리움 컨트랙트가 해커를 위한 사탕이 될 것 이라는 또 다른 게시물을 참고하라.

## Tx.Origin 인증

솔리디티에는 글로벌 변수 tx.origin이 있다. 이는 전체 호출 스택을 가로지르고 원래 호출을 보낸 계정의 주소를 포함한다. 스마트 컨트랙트에서 이 변수를 인증에 사용하면 컨트랙트가 피싱 같은 공격에 취약해진다.

### 취약점

tx.origin 변수를 사용하여 사용자에게 권한을 부여하는 컨트랙트는 일반적으로 사용자로 하여금 취약한 컨트랙트에서 인증된 작업을 수행하도록 속일 수 있는 피싱 공격에 취약하다.

``` solidity
contract Phishable {
    address public owner;

    constructor (address _owner) {
        owner = _owner;
    }

    function () public payable {}

    function withdrawAll(address _recipient) public {
        require(tx.origin == owner);
        _recipient.transfer(this.balance);
    }
}
```

11행에서 컨트랙트는 tx.origin을 사용하여 withdrawAll 함수를 승인한다. 이 컨트랙트를 통해 공격자는 다음 형식의 공격 컨트랙트를 작성할 수 있다.

``` solidity
import "Phishable.sol";

contract AttackContract {
    phishable phishableContract;
    address attacker;

    constructor (PhishableContract, address _attackerAddress) {
        phishableContract = _phishableContract;
        attacker = _attackerAddress;
    }

    function () payable {
        phishableContract.withdrawAll(attacker);
    }
}
```

공격자는 자신의 개인 주소나 피해자가 주소로 어떤 형태의 트랜잭션을 보내도록 소셜 엔지니어 피해자로서 컨트랙트를 위장할 수 있다. 아마도 어느 정도 양의 이더를 보내는 컨트랙트일 것이다. 피해자는 조심하지 않으면 공격자의 주소에 코드가 있음을 알아채지 못할 수도 있다. 공격자가 다중 서명 지갑이나 고급 스토리지 지갑으로 전달할 수도 있다.

### 예방 기법

tx.origin은 스마트 컨트랙트에서 권한을 위해 사용되어서는 안 된다. tx.origin 변수를 절대로 사용해서는 안 된다는 말은 아니다. 스마트 컨트랙트에서 정당한 사용 사례가 있다. 예를 들어, 외부 컨트랙트가 현재 컨트랙트를 호출하는 것을 거부하려면 require(tx.origin == msg.sender) 형식의 요구사항을 구현할 수 있다. 이렇게 하면 다른 중간 단계의 컨트랙트들이 해당 컨트랙트를 호출하지 못하게 함으로써, 이 컨트랙트는 코드가 없는 일반 주소만이 호출할 수 있게 된다.

## 컨트랙트 라이브러리

호출 가능한 라이브러리로 온체인에 배포되어 있거나 코드 샘플 라이브러리로 오프체인에 배포된 형태로서 재사용 가능한 많은 코드가 있다. 플랫폼상에 이미 배포된 라이브러리는 바이트코드 스마트 컨트랙트 형태다. 그래서 프로덕션에서 사용하기 전에 매우 주의해야 한다. 그러나 잘 구축된 기존의 온플랫폼 라이브러리를 사용하면 최신 업그레이드 혜택을 누릴 수 있는 등의 많은 이점을 얻을 수 있으며, 이더리움의 실제 컨트랙트 수를 줄임으로써 비용을 절약하고 이더리움 생태계에 이익을 줄 수 있다.

이더리움에서 가장 많이 사용되는 자원은 오픈제플린 스위트로, ERC20 및 ERC721 토큰의 구현에서부터 많은 종류의 크라우드세일 모델에 이르기까지, Ownable, Pausable 또는 LimitBalance 같은 컨트랙트에서 일반적으로 알려진 단순한 동작에 이르기까지 광범위한 라이브러리다. 이 저장소의 컨트랙트는 광범위한 테스트를 거치고 사실상 표준 구현으로서 기능을 수행하는 경우도 있다. 무료로 사용할 수 있으며, 지속적으로 증가하는 외부 기여자와 함께 제플린에 의해 구축되고 관리된다.

또한 제플린의 제플린OS는 오픈 소스 서비스 플랫폼으로 스마트 컨트랙트 애플리케이션을 안전하게 개발하고 관리하기 위한 도구다. 제플린 OS는 EVM 상단에 레이어를 제공하며, 개발자가 잘 테스트된 컨트랙트의 온체인 라이브러리에 링크되는 업그레이드 가능한 댑을 쉽게 시작할 수 있게 한다. 이 라이브러리들의 각기 다른 버전들은 이더리움 플랫폼상에 공존할 수 있으며, 사용자는 보상 시스템을 통해 개선 방향을 제시하거나 향상할 수 있다. 탈중앙화 애플리케이션을 디버깅, 테스트, 배포 및 모니터링하는 오프체인 도구 모음도 플랫폼에서 제공한다.

프로젝트 ethpm은 패키지 관리 시스템을 제공하여 생태계에서 개발하고 있는 다양한 자원을 체계화하는 것을 목표로 한다.