# 트러플 기초와 단위 테스팅

트러플은 개발, 테스트, 배포를 쉽게 할 수 있게 도와주는 유틸리티다.

### 애플리케이션 개발 생명주기 관리

애플리케이션을 진지하게 개발할 때는 구축 프로세스를 따르기 마련이다. 일반적인 구축 프로세스는 설계, 구축, 테스트 배포의 순서로 이뤄진다. 계약의 수명 주기를 관리하는 것도 다른 소프트웨어 혹은 프로그래밍을 개발할 때와 크게 다르지 않다.

계약 개발

1. 문제와 관련된 요구사항을 수집, 확정.
2. 프로젝트 개발팀은 이러한 아키텍처와 설계 문서를 바탕으로 기능을 구현.
3. 단위 테스트를 통해 평가.


### 트러플

``` cmd
$ npm install -g truffle
```

### 트러플을 가지고 개발하기

1. 모든 프로젝트 및 트러플에서 만들어지는 산출물을 저장할 project 폴더를 생성하는 것이다.
2. 그 폴더로 이동해서 init 명령을 입력한다. init 명령은 해당 폴더에서 트러플의 초기화를 수행한다. 즉, 필요한 폴더, 코드 파일, 설정, 링크를 생성한다.

폴더 구조

- contract : migrations.sol이라는 파일이 있다. 이 곳에는 이더리움 네트워크에 맞춤 계약을 배포하는 계약이 있다. 맞춤 계약은 이 폴더에 위치하게 된다.
- migrations : 계약 배포 프로세스를 실행하는 여러 개의 자바스크립트 파일이 있다. 이러한 자바스크립트 파일은 트러플에서 모든 맞춤 계약을 볼 수 있도록 수정돼야 하며, 배포를 위해 트러플이 올바른 순서로 체인 및 링크할 수 있게 돼야 한다. 파일명이 숫자로 시작하는 여러 개의 자바스크립트 파일이 들어 있다. 이러한 스크립트들은 1부터 순서대로 실행된다.
- test : 맞춤 테스트 스크립트가 들어가는 곳이다.
- truffle, truffle-config : 프로젝트의 주 설정 파일은 truffle.js.

3. 다음 코드를 네트워크 구성에 사용할 수 있다. 활성화된 RPC 종단과 포트에 게스 인스턴스가 존재해야 한다. JSON-RPC 프로토콜을 사용해 계약을 배포하는 데 게스 대신 ganache-cli를 사용할 수 있다. 기존 이더리움 네트워크에 연결하려면 네트워크 구성 요소를 정의해야 한다. 네트워크의 이름으로 구성하며, 서로 다른 환경의 다중 네트워크를 구성할 수 있다.

``` javascript
module.exports = {
    metworks : {
        development : {
            host: '127.0.0.1',
            port: 8545,
            network_id: '*' // 임의의 네트워크 id와 일치
        }
    }
}
```

4. 새로운 계약을 생성하고 파일명을 first.sol로 해서 contracts 폴더에 저장한다.

``` solidity
pragma solidity ^0.4.17;

contract First {
    int public mydata;

    function GetDouble(int _data) public returns (int _output) {
        mydata = _data * 2;
        return _data * 2;
    }
}
```

5. 또 다른 계약을 작성하고 파일명을 second.sol로 해서 같은 폴더에 저장한다.

``` solidity
pragma solidity ^0.4.17;

import './first.sol';

contract Second {
    address firstAddress;
    int public _data;

    function Second(address _first) public {
        firstAddress = _first;
    }

    function SetData(0 public {
        First h = First(firstAddress);
        _data = h.GetDouble(21);
    })
}
```

6. migrations 폴더에 또 다른 스크립트 파일을 추가한다. 계약 배포 순서를 정하려면 각 파일의 이름이 1씩 증가해야 한다는 데 유의하자. 이 경우에는 파일명을 2_Custom.js로 지정했다. 이 파일의 내용은 다음과 같다. 이 파일의 처음 두 행에서는 앞에서 작성한 두 계약을 참조한다. 이 파일은 배포 중에 트러플에 의해 호출된 함수를 익스포트한다. 그 함수는 먼저 첫 번째 계약을 배포하고, 배포에 성공하면 두 번째 계약을 배포한다.

``` solidity
var hw = artifacts.require("First");

var hw1 = artifacts.require("Second");

module.exports = function(deployer) {
    deployer.deploy(hw).then
        (function() {
            return deployer.deploy(hw1, hw.address);
        })
}
```

7. truffle.cmd를 사용해 compile 명령을 실행한다. 오류와 경고가 발생할 수도 있다. 오류나 경고가 발생한다면 진행하기 전에 그것을 해결해야 한다.

``` cmd
$ truffle.cmd compile
```

8. 컴피아리한 계약을 배포할 차례다. 트러플은 migrate 명령을 제공한다. migrate 명령을 실행하기 전에 게스 또는 가나슈 인스턴스가 구동되고 있어야 한다. 게스 채굴의 경우 채굴 프로세스도 작동하고 있어야 한다. testrpc를 사용하는 경우에는 채굴자가 필요하지 않다.


### 트러플을 가지고 테스트하기

단위 테스트 => 통합 테스트
테스트는 test 폴더에 들어가며, 솔리디티와 자바스크립트로 작성할 수 있다. 계약의 이름으로는 테스트 대상 계약의 이름 앞에 Test를 붙이며, 함수의 이름으로는 테스트 대상 함수의 이름 앞에 test를 붙인다. 대문자와 소문자를 구분한다는 점에 유의하자.

``` solidity
pragma solidity ^.0.4.19;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/first.sol";

contract TestFirst {
    function testGetDoublePositiveUsingDeployedContract() {
        First meta = First(DeployedAddresses.First());
        Assert.equal(meta.GetDouble(10), 20, "Positive input gives double value");
    }
}
```

