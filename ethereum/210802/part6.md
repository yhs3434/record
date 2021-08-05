# 트랜잭션

## 논스의 간격, 중복 논스 및 확인

이더리움 네트워크는 논스에 따라 트랜잭션을 순차적으로 처리한다. 즉, 논스가 0인 트랜잭션을 전송한 다음 논스가 2인 트랜잭션을 전송하면, 두 번 째 트랜잭션은 어떤 블록에도 포함되지 않는다. 이더리움 네트워크가 누락된 논스가 나타날 때 까지 기다리는 동안 두 번째 트랜잭션은 멤풀에 저장된다. 모든 노드는 누락된 논스가 단순히 지연되었고 논스가 2인 트랜잭션은 순서가 맞지 않게 수신되었다고 가정한다.

만약, 예를 들어 논스가 같지만 수신자나 값이 다른 2개의 트랜잭션을 전송하는 것과 같은 논스의 중복이 일어나면, 그 중 하나가 확정되고 하나는 거부된다. 어떤 트랜잭션이 확정되는지는 그 트랜잭션이 첫 유효 노드에 도달하는 순서에 따라 결정된다. 즉, 이는 무작위적이다.

## 동시 실행, 트랜잭션 생성 및 논스

동시 실행은 컴퓨터 과학이 다루어야 하는 복잡한 문제이며, 때때로 예상치 못하게 발생할 수도 있는데, 특히 이더리움처럼 탈중앙화되어 있고 분산되어 있는 실시간 시스템에서는 더욱 그러하다.

간단히 말해, 동시 실행 문제는 여러 독립 시스템에 의한 동시적인 계산이 있는 경우다. 이들은 동일한 프로그램(예: 멀티 쓰레딩), 동일한 CPU 또는 다른 컴퓨터에 있을 수 있다. 이더리움은 정의상 작업의 동시 실행을 허용하지만 합의를 통해 싱글톤 상태를 강제하는 시스템이다.

## 트랜잭션 가스

가스는 이더리움의 연료다. 가스는 이더가 아니라 이더에 대한 자체 환율을 가진 별도의 가상화폐다. 이더리움은 가스를 사용하여 트랜잭션이 사용할 수 있는 자원의 양을 제어한다. 이는 전 세계 수천 대의 컴퓨터에서 처리되기 때문이다. 개방형 계산 모델은 DoS 공격이나 실수로 막대한 자원을 소모하는 트랜잭션을 피하기 위해 특정한 형태의 미터링이 필요하다.

가스는 이더 가치의 급변한 변화와 함께 발생할 수 있는 변동성으로부터 시스템을 보호하고, 가스가 지급하는 다양한 자원의 비용 사이의 중요하고 민감한 비율을 관리하기 위해 가스를 이더와 분리한다.

트랜잭션의 gasPrice 필드는 트랜잭션 생성자가 가스와 교환하여 지급할 가격을 설정할 수 있게 한다. 가격은 가스 단위당 웨이 단위로 측정된다. 예를 들어, 2장의 샘플 트랜잭션에서 지갑은 gasPrice를 3Gwei로 설정한다.

지갑은 신속한 트랜잭션 컨펌을 위해 gasPrice를 조정할 수 있다. gasPrice가 높을수록 트랜잭션이 더 빨리 컨펌될 것이다. 반대로, 우선순위가 낮은 트랜잭션은 낮은 가격을 설정해서 컨펌이 느려지게 할 수 있다. gasPrice가 설정될 수 있는 최솟값은 0이고, 이것은 수수료 없는 트랜잭션을 의미한다. 블록 공간에 대한 수요가 낮은 기간에는 수수료가 0인 트랜잭션들도 블록에 포함될 수 있다.

web3 인터페이스는 여러 블록에 걸친 중간 가격을 계산하여 getPrice를 제안하는 기능을 제공한다.

``` javascript
web3.eth.getGasPrice(console.log);
```

가스와 관련된 두 번째 중요한 필드는 gasLimit이다. 간단히 말하면, gasLimit은 ~~.
gas의 양은 21000개로 고정 됨. (why??)

트랜잭션 목적지 주소가 컨트랙트인 경우, 필요한 가스양을 추정할 수는 있지만 정확하게 결정할 수는 없다. 이는 컨트랙트가 각기 다른 실행 경로로 이어지는 조건을 가질 수 있어 총 가스 비용이 다를 수 있기 때문이다. 컨트랙트는 통제할 수 없거나 예측할 수 없는 조건들에 의해 단순한 계산으로 끝날 수도 있고, 더 복잡한 계산을 수행해야 할 수도 있다. 예를 한 번 들어보자. 호출될 때 마다 카운터를 증가시키고 호출 횟수와 같은 횟수만큼 특정 루프를 실행하는 스마트 컨트랙트를 작성할 수 있다. 100번째 호출에서 복권과 같은 특별한 상을 줄 수도 있지만, 그것을 계싼하려면 추가적인 연산이 필요하다. 컨트랙트를 99번 호출하는 동안 결과가 있었고, 100번째 호출에서 매우 다른 일이 발생한다. 지급할 가스의 양은 트랜잭션이 블록에 포함되기 전에 얼마나 많은 다른 트랜잭션이 해당 기능을 호출했는지에 따라 다르다. 여러분의 추정치는 99번째 트랜잭션을 기반으로 하지만, 트랜잭션이 확인되기 직전에 다른 사람이 99번째 컨트랙트를 요청한다. 이제 여러분은 호출할 수 있는 100번째 트랜잭션이고 계산 작업은 훨씬 더 높다.

이더리움에서 사용되는 일반적인 비유를 빌리자면 gasLimit를 자동차의 연료 탱크 용량으로 생각할 수 있다. 여행에 필요한 만큼의 가스를 탱크에 채운다. 금액을 어느 정도 예측할 수 있지만, 연료 소비를 증가시키는 우회 같은 예기치 않은 변경사항이 있을 수 있다.

트랜잭션이 완료될 때 까지는 금액이 실제로 차감되지는 않는다. 하지만 트랜잭션을 보내기 전에 완료까지 진행할 잔액이 있는지 확인해야 한다.

## 트랜잭션 수신자

to 필드 : 20바이트 이더리움 주소. (EOA or 컨트랙트 주소)

이더리움은 이 필드를 더는 검증하지 않는다. 모든 20바이트 값은 유효한 것으로 간주한다. 따라서 유효성 검사를 UI 수준에서 수행되어야 한다.

## 트랜잭션 값과 데이터

트랜잭션의 주요 페이로드는 값과 데이터라는 2개의 필드에 포함된다. 트랜잭션은 값과 데이터, 값만, 데이터만 또는 값이나 데이터를 모두 가지지 않는 네가지 조합이 모두 유효하다.

값만 있는 트랜잭션은 지급이다. 데이터만 있는 트랜잭션은 호출이다. 값과 데이터 모두를 사용한 트랜잭션은 지급과 호출이다. 값과 데이터가 모두 없는 트랜잭션은 단지 가스 낭비일 뿐이다. 그러나 가능은 하다.

이후 실습.

## EOA 및 컨트랙트에 값 전달

값을 포함하는 이더리움 트랜잭션을 구성하면 '지급'과 동일하다. 이러한 트랜잭션은 대상 주소가 컨트랙트인지 여부에 따라 다르게 작동한다.

EOA 주소의 경우 또는 블록체인의 컨트랙트로 표시되지 않는 주소의 경우 이더리움은 상태 변경을 기록하여 주소 잔액에 보낸 값을 추가한다. 이전에 주소가 표시되지 않은 경우, 클라이언트 내부 상태 표현에 추가되고 잔액은 지급 금액으로 초기화된다.

목적지 주소(to)가 컨트랙트라면 EVM은 컨트랙트를 실행하고 트랜잭션의 데이터 페이로드에 지정된 함수를 호출하려고 시도한다. 트랜잭션에 데이터가 없으면 EVM은 폴백 함수를 호출하고, 해당 함수가 지급 가능하다면 다음에 수행할 작업을 결정하기 위해 함수를 실행한다. 폴백 함수가 없다면 트랜잭션의 효과는 지갑에 지급하는 것과 마찬가지로 컨트랙트의 잔액을 늘린다.

컨트랙트는 함수가 호출될 때 또는 함수에 코딩된 조건에 따라 즉시 예외를 발생시켜 입금을 거부할 수 있다. 함수가 예외 없이 성공적으로 끝나면, 컨트랙트의 상태가 이더 잔액이 증가했음을 반영하여 업데이트 된다.

## EOA 또는 컨트랙트에 데이터 페이로드 전달

트랜잭션에 데이터가 포함되어 있으면 받는 주소는 컨트랙트 주소가 될 가능성이 크다. 그렇다고 해서 이더리움 프로토콜에서 완전히 유효한 데이터 페이로드를 EOA에 보낼 수 없다는 뜻은 아니다. 그러나 이 경우 데이터 해석은 EOA에 접근하는 데 사용하는 지갑에 달려 있다. 그것은 이더리움 프로토콜에 의해 무시된다. 또한 대부분의 지갑은 자신이 제어하는 EOA에 대한 트랜잭션에서 수신된 모든 데이터를 무시한다.

미래에는 지갑이 컨트랙트 방식대로 데이터를 해석할 수 있도록 하는 표준이 등장할 수 있으므로 트랜잭션은 사용자 지갑 내부에서 실행되는 함수를 호출할 수 있다. 중요한 차이점은 EOA에 의한 데이터 페이로드의 해석은 컨트랙트 실행과 달리 이더리움의 합의 규칙의 적용을 받지 않는다는 것이다.

트랜잭션이 컨트랙트 주소로 데이터를 전달한다고 가정해 보자. 이 경우 데이터는 EVM에 의해 컨트랙트 호출로서 해석된다. 대부분의 컨트랙트에서는 이 데이터를 함수 호출로 사용하며, 명명된 함수를 호출하고 인코딩된 인수를 함수에 전달한다.

ABI 호환 컨트랙트로 전송된 데이터 페이로드는 다음을 16진수로 시리얼라이즈한 인코딩이다.

- 함수 선택기

    함수 선택기는 함수 프로토타입의 Keccak-256 해시의 처음 4바이트다. 이렇게 하면 컨트랙트에서 호출할 함수를 정확하게 식별할 수 있다.

- 함수 인수

    함수의 인수는 ABI 사양에 정의된 다양한 기본 유형에 대한 규칙에 따라 인코딩된다.

``` solidity
function withdraw(uint withdraw_amount) public {

}
```

함수의 프로토타입은 함수의 이름을 포함하는 문자열로 정의되고, 각 인수의 데이터 유형이 괄호 안에 들어 있으며, 쉼표로 구분된다. 함수 이름은 withdraw이며, 여기에는 uint인 단일 인수가 사용되므로 withdraw의 프로토타입은 다음과 같다.

```
withdraw(uint256)
```

이하 sha3 으로 인코딩한 값.

## 특별 트랜잭션 : 컨트랙트 생성

우리가 언급해야 할 특별한 경우 중 하나는 블록체인에 새로운 컨트랙트를 만들어 향후 사용을 위해 배포하는 트랜잭션이다. 컨트랙트 생성 트랜잭션은 제로 어드레스라고 하는 특수 대상 주소로 전송된다. 컨트랙트 등록 트랜잭션의 to 필드는 0x0 주소를 포함한다. 이 주소는 EOA나 컨트랙트를 나타내지 않는다. 결코 이더를 소비하거나 트랜잭션을 시작할 수 없다. 이 필드는 목적지로만 사용되며, '컨트랙트 작성'이라는 특별한 의미로 사용된다.

제로 어드레스는 컨트랙트 생성에만 사용하려는 의도로 만들어졌지만, 때로는 제로 어드레스를 목적지로 하는 다양한 트랜잭션이 있다. 이것에 대해서는 두 가지로 설명할 수 있다. 즉, 실수로 인한 이더 손실이나 의도적인 이더 연소다. 그러나 이더의 의도적인 연소를 원한다면 네트워크에 의도를 분명히 하고 대신 지정된 주소를 사용해야 한다.

컨트랙트 생성 트랜잭션은 컨트랙트를 생성할 컴파일된 바이트코드를 포함하는 데이터 페이로드만 포함하면 된다. 이 트랜잭션의 유일한 효과는 컨트랙트를 작성하는 것이다. 새 컨트랙트를 특정 잔액으로 설정해서 시작하려면 값 필드에 이더 금액을 포함할 수 있지만, 이는 전적으로 선택사항이다. 데이터 페이로드 없이 컨트랙트 생성 주소에 값을 보내면, 그 효력은 이더를 연소 주소로 전송하는 것과 같다. 트랜잭션이 일어날 컨트랙트가 없기 때문에 이더를 잃게 된다.

예를 들어, 데이터 페이로드에서 컨트랙트를 사용하여 트랜잭션을 수동으로 생성해서 2장에서 사용되는 Faucet.sol 컨트랙트를 생성할 수 있다. 컨트랙트는 바이트코드 표현으로 컴파일 해야 한다. 이것은 솔리디티로 컴파일할 수 있다.

이하 cmd 명령어.

## 디지털 서명

디지털 서명이 작동하는 방식과 디지털 서명이 개인키를 공개하지 않고 어떻게 개인키의 소유권을 증명하는지 살펴본다.

### 타원 곡선 디지털 서명 알고리즘

디지털 서명은 이더리움에서 세 가지 용도로 사용된다. 첫 째, 서명은 이더리움 계정과 개인키의 소유자가 이더 지출 또는 컨트랙트 이행을 승인했음을 증명한다. 둘째, 부인 방지를 보장한다. 즉 허가의 증거는 부인할 수 없다. 셋째, 서명은 트랜잭션이 서명된 후에는 트랜잭션 데이터가 수정되지 않았고 어느 누구도 트랜잭션 데이터를 수정할 수 없음을 증명한다.

in 위키피디아

디지털 서명은 디지털 메시지나 문서의 진위를 표현하기 위한 수학적 기법이다. ~~~

### 디지털 서명 작동 방법

디지털 서명은 두 단계로 구성된 수학적 체계다. 첫 번째 부분은 메시지에서 개인키를 사용하여 서명을 만드는 알고리즘이다. 두 번째 부분은 누구나 메시지와 공개키만 사용하여 서명을 검증할 수 있게 해주는 알고리즘이다.

#### 디지털 서명 만들기

이더리움의 ~~ 이하 실습.

### 서명 확인

서명을 확인하려면 서명(r 및 s)과 시리얼라이즈된 트랜잭션, 그리고 서명을 만드는 데 사용된 개인키에 상응하는 공개키가 있어야 한다. 본질적으로, 서명 확인은 공개키를 생성한 개인키의 소유자만이 트랜잭션에서 서명을 생성할 수 있음을 의미한다.

서명 검증 알고리즘은 메시지, 서명자의 공개키 및 서명(r 및 s 값)을 가져와서 서명이 메시지와 공개키에 유효하면 true를 반환한다.

### ECDSA 계산

이하 계산 방식의 수학적 접근. 머리 아퍼~

### 트랜잭션 서명 실습

이하 실습

### 원시 트랜잭션 생성 및 서명

이하 코드

### EIP-155를 사용한 원시 트랜잭션 생성

EIP-155 '단순 재생 공격 방지' 표준은 서명하기 전에 트랜잭션 데이터 내부에 체인 식별자를 포함하여 재생 공격 방지가 가능한 트랜잭션 인코딩을 지정한다. 이렇게 하면 하나의 블록체인에 대해 생성된 트랜잭션이 다른 블록체인에서 유효하지 않다. 따라서 표준의 이름 그대로 한 네트워크에서 전파된 트랜잭션은 다른 네트워크에서 재생될 수 없다.

EIP-155는 트랜잭션 데이터 구조의 주요 6개 필드에 체인 식별자, 0, 0의 3개 필드를 추가한다. 이 세 필드는 인코딩되고 해싱되기 전에 트랜잭션 데이터에 추가된다. 따라서 트랜잭션의 해시가 변경되어 나중에 서명이 적용된다. 체인 식별자가 서명된 데이터에 포함됨으로써, 트랜잭션 서명은 체인 식별자가 수정되면 서명이 무효화되어 데이터 변경을 식별할 수 있다. 따라서 EIP-155는 서명의 유효성이 체인 식별자에 의존하기 때문에 다른 체인에서 트랜잭션을 재생할 수 없다.

체인 식별자 필드

- 이더리움 메인넷 : 1
- 모던, 확장 : 2
- 롭스텐 : 3
- 린케비 : 4
- 루트스톡 메인넷 : 30
- 루트스톡 테스트넷 : 31
- 코반 : 42
- 이더리움 클래식 메인넷 : 61
- 이더리움 클래식 테스트넷 : 62
- 게스 사설 테스트넷 : 1337

결과로 생성되는 트랜잭션 구조는 RLP로 인코딩되고 해싱되고 서명된다. 서명 알고리즘은 v 접두어에 체인 식별자를 인코딩하기 위해 약간 수정된다.

## 서명 접두어 값(v) 및 공개키 복구

서명자의 공개키를 복구하는 프로세스를 공개키 복구라고 한다.

이하 수학 공식

## 서명 및 전송 분리(오프라인 서명)

트랜잭션이 서명되면 트랜잭션은 이더리움 네트워크로 전송할 준비가 된다. 트랜잭션 생성, 서명, 브로드캐스트의 세 단계는 일반적으로 단일 작업에 의해 처리된다. 그러나 '원시 트랜잭션 생성 및 서명' 절에서 봤듯이, 두 단계로 나누어 트랜잭션을 생성하고 서명할 수 있다. 서명된 트랜잭션이 있으면 web3.eth.sendSignedTransaction을 사용하여 트랜잭션을 16진수로 인코딩하고 서명해서 이더리움 네트워크에서 전송할 수 있다.

트랜잭션의 서명과 전송을 분리하려는 이유는 무엇인가? 가장 보편적인 이유는 보안이다. 트랜잭션에 서명하는 컴퓨터에는 잠금 해제된 개인키가 메모리에 로드되어 있어야 한다. 전송을 수행하는 컴퓨터는 인터넷에 연결되어 있어야 하며, 이더리움 클라이언트를 실행해야 한다. 이 두 기능이 하나의 컴퓨터에 있으면 온라인 시스템에 개인키가 있게 되며, 이는 매우 위험한 상황이 된다. 서명 및 전송 기능을 분리하여 각기 다른 시스템에서 수행하는 것을 오프라인 서명이라고 하며, 이는 일반적인 보안 방법이다.

## 트랜잭션 전파

노드는 P2P 네트워크에 연결되어 참여하는 이더리움 클라이언트를 지칭한다.

## 블록체인에 기록하기

이더리움의 모든 노드는 동등한 피어이지만, 일부는 채굴을 하며 고성능 그래픽 처리 장치가 장착된 컴퓨터인 채굴 팜에 트랜잭션 및 블록을 제공한다. 채굴 컴퓨터는 트랜잭션을 후보 블록에 추가하고 후보 블록을 유효하게 만드는 작업증명을 찾으려고 시도한다.

유효한 트랜잭션이 결국 트랜잭션 블록에 포함되어 이더리움 블록체인에 기록된다. 트랜잭션이 블록으로 채워지면 계정의 잔액을 수정하거나 내부 상태를 변경하는 컨트랙트를 호출하여 트랜잭션은 이더리움 싱글톤 상태를 수정한다. 이러한 변경사항은 이벤트가 포함될 수 있는 트랜잭션 영수증 형식으로 트랜잭션과 함께 기록된다.

생성 -> 서명 -> 전파 -> 채굴 -> 블록체인에 지울 수 없는 기록을 남김.

## 다중 서명 트랜잭션

비트코인의 스크립팅 기능에 익숙하다면, 여러 당사자가 트랜잭션에 서명할 때만 자금을 사용할 수 있는 비트코인 다중 서명 계정을 만들 수 있음을 알고 있을 것이다. 이더리움의 기본 EOA 값 트랜잭션에는 다중 서명 조항이 없다. 그러나 이더와 토큰을 전송하는 어떤 조건들도 처리할 수 있는 스마트 컨트랙트를 사용해 임의의 서명 제한룰을 적용할 수 있다.
