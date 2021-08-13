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

