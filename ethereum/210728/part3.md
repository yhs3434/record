# 이더리움 클라이언트

이더리움 클라이언트들끼리 상호운용이 가능하다.
이더리움은 오픈소스 프로젝트이다.

비트코인의 '사양'이 비트모인 코어 기준 구현체인 반면, 이더리움은 영어와 수학 사양이 결합되어 문서화되어 있다. 이더리움은 각 클라이언트가 구현해야 될 사양에 대해 명확한 기준을 제시하고 있기 때문에, 독립적으로 개발된 클라이언트들도 대부분 상호운용이 가능하다.

이더리움은 다른 블록체인보다 네트워크에서 실행되는 구현의 다양성이 더 크며, 이는 일반적으로 좋은 점으로 평가된다. 실제로 이더리움은 네트워크 공격을 방어하기 위한 훌륭한 방법으로 입증되었다. 왜냐하면 특정 클라이언트의 구현 전략을 악용하는 것은 개발자가 공격을 패치하는 동안 개발자를 괴롭히지만, 다른 클라이언트는 네트워크에 거의 영향을 주지 않기 때문이다.

## 이더리움 네트워크

이더리움 기반 네트워크는 이더리움, 이더리움 클래식, 엘라, 익스팬스, 유비크, 뮤지코인 등 기타 여러 네트워크가 있다.

현재 여섯 가지 언어로 작성된 이더리움 프로토콜의 여섯 가지 기본 구현이 있다.

- 러스트로 작성된 패리티
- 고로 작성된 게스
- C++로 작성된 cpp-ethereum
- 파이썬으로 작성된 pyethereum
- 스칼라로 작성된 맨티스
- 자바로 작성된 하모니

## 풀 노드를 실행해야 하는가?

블록체인의 건전성, 복원력, 검열 저항 특성은 독립적으로 운용되고 지리적으로 분산된 풀 노드가 얼마나 많은지에 달려있다. 각각의 풀 노드는 다른 새 노드가 블록 데이터를 얻어 작업을 부트스트랩하고, 운영자에게 모든 트랜잭션 및 컨트랙트에 대해 신뢰할 수 있고 독립적인 검증을 제공할 수 있도록 도와준다.

일반적으로 원격 클라이언트는 지갑의 트랜잭션 기능 외에도 API를 제공한다.
이더리움의 원격 지갑 개념을 라이트 클라이언트의 개념과 혼동하면 안된다. 라이트 클라이언트는 블록 헤더의 유효성을 검사하고, 머클 증명을 사용하여 블록체인에서의 트랜잭션 포함 여부를 확인하고, 그 영향도를 결정하여 풀 노드에 대해 비슷한 수준의 보안을 제공한다. 반대로, 이더리움 원격 클라이언트는 블록 헤더 또는 트랜잭션의 유효성을 검사하지 않는다. 그들은 풀 클라이언트를 신뢰하여 블록체인에 대한 접근 권한을 부여하므로, 보안 및 익명성 보장 수준이 상당히 떨어진다. 여러분 스스로 실행하는 풀 클라이언트를 사용하여 이러한 문제를 완화할 수 있다.

## 풀 노드의 장단점

장점

- 이더리움 기반 네트워크의 복원력과 검열 저항을 지원함.
- 모든 트랜잭션을 정식으로 검증함.
- 중개자 없이 공개 블록체인의 모든 컨트랙트와 상호작용할 수 있음.
- 중개자 없이 컨트랙트를 공개 블록체인에 직접 배포할 수 있다.
- 블록체인 상태를 오프라인에서 조회 가능.
- 정보를 제 3자에게 노출하지 않고 가져올 수 있음.

단점

- 하드웨어와 대역폭 자원의 확대가 필요.
- 처음 시작할 때 전체 동기화를 위해 여러 날이 소요.
- 동기화를 유지하기 위해 관리하고, 업그레이드 하고, 온라인 상태로 유지해야 함.

## 공개 테스트넷의 장단점

장점

- 테스트넷 노드는 훨씬 적은 데이터와 동기화를 필요로 함.
- 테스트넷 노드는 몇 시간 내에 전체 동기화를 할 수 있음.
- 컨트랙트 배포 및 트랜잭션 생성을 위한 테스트용 이더를 몇몇 Faucet으로부터 무료로 얻을 수 있음.
- 테스트넷은 다른 많은 스마트 컨트랙트가 동작 실행중인 공개 블록체인임.

단점

- 테스트넷에서 실제 돈을 사용할 수 없음.
- 테스트넷에서는 퍼블릭 블록체인에서만큼 실전 테스트를 할 수 없는 측면이 있음. 예를들면, 테스트넷에서는 실제 트랜잭션 시 필수적으로 발생하는 비용인 가스 수수료를 고려하지 않음.

## 로컬 블록체인 시뮬레이션 장단점

장점

- 동기화가 없고, 디스크에 데이터가 거의 없음.
- 테스트 이더를 얻을 필요가 없음.
- 다른 사용자는 없고 단지 본인만 있음.
- 다른 컨트랙트는 없고 단지 본인이 시작하고 배포한 컨트랙트만 있음.

단점

- 다른 사용자가 없다는 건, 공개 블록체인과 동일하게 동작하지 않는다는 뜻임. 따라서 트랜잭션 순서나 트랜잭션 공간을 두고 경쟁이 없음.
- 본인 말고 채굴자가 없다는건, 채굴이 더욱 예측 가능하다는 뜻임. 그러므로 본인은 공개 블록체인에서 발생하는 일부 시나리오를 테스트할 수 없음.
- 다른 컨트랙트가 없다는 건, 테스트를 위해 의존성을 갖는 것들과 컨트랙트 라이브러리를 포함하여 본인이 원하는 모든 것을 배포해야 한다는 뜻임.
- 본인은 어떤 시나리오를 테스트하기 위한 공개 컨트랙트와 주소를 다시 만들지 못할 수도 있다.

## 이더리움 클라이언트 실행

테스트넷 노드로 풀 노드를 실행하는 것을 선택하든지 아니면 로컬 사설 블록체인을 위한 클라이언트를 선택하든지 간에, 이러한 클라이언트를 설치해보는 것은 도움이 될 만한 일이다.

### 풀 노드를 위한 하드웨어 요구사항

이더리움 블록체인의 전체 사본을 저장하기 위해서는 적어도 80gb의 디스크 공간이 필요하다. 만약 이더리움 테스트넷에서 풀 노드를 실행하기를 원한다면 적어도 15GB가 더 필요하다. 80GB 블록체인 데이터를 다운로드하는 데는 오랜 시간이 걸린다.

이더리움 블록체인을 동기화하는 것은 매우 많은 입출력을 필요로 한다. 가장 좋은 방법은 SSD를 준비하는 것이다. 만약 기계식 HDD를 갖고 있다면 캐시를 사용하기 위해 적어도 8GB 램이 필요할 것이다.

### 클라이언트(노드)를 빌드하고 실행하기 위한 소프트웨어 요구사항

일반적으로 모든 블록체인에는 자체 버전의 게스가 있다. 패리티는 동일한 클라이언트 다운로드를 통해 여러 이더리움 기반 블록체인을 지원한다.

### 패리티

패리티는 풀 노드 이더리움 클라이언트 및 댑 브라우저를 구현한 것이다. 패리티는 모듈식의 안전하고 확장 가능한 이더리움 클라이언트를 구축하기 위해 시스템 프로그래밍 언어인 러스트를 기반으로 작성되었다. 패리티는 영국 회사인 패리티 테크에서 개발했으며, GPLv3 무료 소프트웨어 라이선스하에 출시되었다.

### 패리티 설치

실습.

### 게스

게스는 이더리움 재단에서 고 언어로 개발하였으며, 이더리움의 '공식' 클라이언트다. 일반적으로 모든 이더리움 기반 블록체인은 자체 게스 구현을 갖고 있다. 만약 게스를 실행하고 있다면, 다음 저장소 링크 중 하나를 사용하여 블록체인에 적합한 버전을 찾아야 한다.

### 이더리움 기반 블록체인의 첫 번째 동기화

일반적으로 이더리움 블록체인을 동기화할 때 클라이언트는 처음부터, 즉 제네시스 블록 이후 모든 블록 및 트랜잭션을 다운로드하고 유효성을 검사한다.

블록체인을 이런 식으로 완전히 동기화하는 것은 가능하지만, 동기화는 매우 오래 걸리고 많은 자원을 필요로 한다.

## JSON-RPC 인터페이스

이더리움 클라이언트는 애플리케이션 인터페이스와 JSON으로 인코딩된 RPC 명령을 제공한다. 여러분은 JSON-RPC API를 참고하여 이 명령을 볼 수 있을 것이다. 기본적으로 JSON-RPC API는 이더리움 클라이언트를 이더리움 네트워크 및 블록체인의 게이트웨이로 사용하는 프로그램을 작성할 수 있게 해주는 인터페이스다.

일반적으로 RPC 인터페이스는 포트 8545에서 HTTP 서비스로 제공된다. 기본적으로 보안상의 이유로 로컬 호스트의 연결만 허용하도록 제한된다.

JSON-RPC API에 접근하려면 사용 가능한 각 RPC 명령에 해당하는 'stub' 함수 호출을 제공하는 특수 라이브러리를 사용하거나, HTTP 요청과 JSON으로 인코딩된 보내기/받기 요청을 수동으로 생성할 수 있다. curl 같은 일반적인 커맨드 라인 HTTP 클라이언트를 사용하여 RPC 인터페이스를 사용하여 RPC 인터페이스를 호출할 수도 있다. 먼저 게스가 구성되어 실행 중인지 확인한 다음, 새 터미널 창으로 전환하여 시도해본다.

## 원격 이더리움 클라이언트

원격 클라이언트는 풀 클라이언트의 일부 기능을 제공한다. 원격 클라이언트는 더 빠르며 데이터 용량도 훨씬 더 적게 요규한다.

### 모바일 지갑

모든 모바일 지갑은 원격 클라이언트다.

### 브라우저 지갑

다양한 지갑과 댑 브라우저는 크롬 및 파이어폭스 같은 웹 브라우저의 확장 혹은 플러그인 형태로 사용이 가능하다.