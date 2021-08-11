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

