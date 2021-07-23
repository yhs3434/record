### 스마트 계약

스마트 계약(smart contract)이란 무엇일까? 많은 사람들이 계약의 의미를 이해하려 애를 쓰며, "스마트"라는 단어를 중요하게 여긴다. 스마트 계약의 본질은 EVM에 배포되어 실행되는 코드 혹은 프로그램이다. 계약이라는 용어는 법률 분야라면 모를까, 프로그래밍 분야와는 그다지 관련이 없다. 솔리디티에서 스마트 계약을 작성한다는 것이 법적 계약을 작성함을 의미하지는 않는다. 다른 프로그래밍 코드와 마찬가지로, 계약은 솔리디티 코드로 이뤄지고, 누군가가 호출하면 실행된다. 여기에 딱히 스마트하다고 할 점은 없다. 스마트 계약이라는 것은 블록체인 분야에서 EVM 내에서 실행되는 프로그래밍 로직과 코드를 가리키는 용어에 불과하다.

스마트 계약은 C++, 자바, C# 클래스와 매우 비슷하다. 클래스에 상태와 행위가 있는 것과 마찬가지로 계약에도 상태 변수와 함수가 있다. 상태 변수의 목적은 계약의 현재 상태를 유지하는 것이고, 함수는 로직을 실행하고 현재 상태를 갱신하거나 읽는 조작을 수행한다.

스마트 계약의 예를 앞장에서 이미 살펴봤지만, 여기서 더 깊이 파고들 것이다.


### 간단한 계약 작성하기

계약을 선언할 때,
contract SampleContract {

}

계약 예제 코드

``` solidity
pragma solidity ^0.4.19;

contract GeneralStructure {
    int public stateIntVariable;
    string stateStringVariable;
    address personIdentifier;
    myStruct human;
    bool constant hasIncome = true;

    modifier onlyBy() {
        if (msg.sender == personIdentifier) {
            _;
        }
    }

    event ageRead(address, int);

    enum gender {male, female};

    function getAge(address _personIdentifier) onlyBy() payable external returns (uint) {
        human = myStruct("Ritesh", 10, true, new uint[](3));

        gender _gender = gender.male;

        ageRead(personIdentifier, stateIntVariable);
    }
}
```

### 계약 생성하기

솔리디티에서는 다음 두 가지 방법으로 계약을 생성하고 사용할 수 있다.

1. new 키워드를 사용
2. 이미 배포한 계약의 주소를 사용


#### new 키워드 사용하기

솔리디티에서 new 키워드는 새로운 계약 인스턴스를 배포 및 생성한다. 그것은 계약을 배포, 상태 변수를 초기화, 생성자를 실행, 논스 값을 1로 설정하고, 최종적으로는 인스턴스의 주소를 호출자에게 반환함으로써 계약 인스턴스를 초기화한다. 계약을 배포하는 것에는 요청과 함께 배포를 완료하기에 충분한 가스가 제공됐는지 확인하고, 요청자의 주소와 논스 값을 가지고 계약 배포를 위한 새로운 계정과 주소를 생성하고, 함께 보내진 이더를 전달하는 일이 포함된다.

``` solidity
pragma solidity ^0.4.19;

contract HelloWorld {
    uint private simpleInt;

    function GetValue() public view returns (uint) {
        return simpleInt;
    }

    function SetValue(uint _value) public {
        simpleInt = _value;
    }
}

contract client {
    address obj;

    function setObject(address _obj) external {
        obj = _obj;
    }

    function UseExistingAddress() public returns (uint) {
        HelloWorld myObj = HelloWorld(obj);
        myObj.SetValue(10);
        return myObj.GetValue();
    }
}
```


### 생성자

솔리디티는 계약에서 생성자를 선언하는 것을 지원한다. 솔리디티에서 생성자는 선택사항이며, 생성자를 명시적으로 선언하지 않으면 컴파일러가 기본 생성자를 넣어준다. 생성자는 계약이 배포될 때만 한 번 실행된다. 이것은 다른 프로그래밍 언어에서와 다른 점이다. 다른 프로그래밍 언어에서는 새로운 객체 인스턴스가 생성될 때마다 생성자가 실행된다. 하지만 솔리디티에서는 EVM에 배포되는 시점에만 실행된다. 생성자는 상태 변수를 초기화하는 데 사용되며, 솔리디티 코드가 많이 들어가는 것은 피해야 한다. 생성자는 계약에서 처음으로 실행되는 코드다. 다른 프로그래밍 언어에서 여러 개의 생성자를 둘 수 있는 것과 달리, 한 계약에 하나의 생성자만 있을 수 있다. 생성자는 파라미터를 취할 수 있으며, 계약을 배포할 때 인자를 넣어야 한다.

생성자는 계약과 같은 이름을 갖는다. 두 이름은 서로 같아야 한다. 생성자의 가시성은 public 또는 internal이 될 수 있지만, external이나 private는 될 수 없다. 생성자는 데이터를 명시적으로 반환하지 않는다. 다음 예에서 HelloWorld 계약의 생성자를 같은 이름으로 정의했다. 생성자에서 스토리지 변숫값을 5로 설정한다.

``` solidity
pragma solidity ^0.4.19;

contract HelloWorld {
    ;uint private simpleInt;

    function HelloWorld() public {
        simpleInt = 5;
    }

    function GetValue() public returns (uint) {
        return simpleInt;
    }

    function SetValue(uint _value) public {
        simpleInt = _simpleInt;
    }
}
```


### 게약 합성

솔리디티는 계약 합성을 지원한다. 문제를 분할해서 여러 건의 계약을 작성한 뒤 계약 합성을 통해 그것들을 엮는 것은 좋은 습관이다.