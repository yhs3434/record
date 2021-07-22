### 솔리디티 파일 구성

솔리디티 파일은 크게 네 부분으로 구성된다.

1. 프라그마
2. 주석
3. 임포트
4. 계약, 라이브러리, 인터페이스


### 계약

계약을 구성하는 요소는 다음과 같다.

- 상태 변수
- 구조체 정의
- 수정자 정의
- 이벤트 선언
- 열거형 정의
- 함수 정의


### 솔리디티의 느낌 (주관적)

함수 안에 함수를 선언하는 느낌은 자바스크립트의 클로저 같다.
상태 변수를 선언할 때, public, private, protected(?) 등의 스코프를 지정할 수 있는 것은 자바의 느낌이다.
상태 변수를 선언할 때, 영속성, 휘발성 등을 선택할 수 있는 것은 c 게열 or 자바의 느낌이다.
구조체 선언은 c 느낌이다.


### 상태 변수

한정자 종류

- internal : default, 선언함 계약 및 해당 계약이 상속된 계약 안에서의 스코프를 가짐. (외부에서 조회는 가능.)
- private : only 그것을 선언한 계약에서만 사용 가능.
- public : 솔리디티 컴파일러는 public 상태 변수 각각에 대한 getter 함수를 생성한다.
- constant : 상수.

자료형 종류

- bool
- uint, int
- bytes
- address
- mapping
- enum
- struct
- string


### 구조체

계약과 비슷하지만, 구조체는 변수만으로 구성된다. (98년 이전의 c와 비슷한 듯.)


### 수정자

수정자는 http 서버로 볼 때 미들웨어와 비슷한 기능을 수행하는 것 같다.
무언가 함수를 실행하기 이전에 실행되는 전처리 함수이다.


### 이벤트

이벤트는 전역 수준에서 계약과 함께 선언되며 그것의 함수 내에서 호출된다.
이벤트 파라미터에 값을 담아서 보낼 수 있다.
이벤트를 이용하면, 계약의 특정 상태에 변화가 있는지 계속 폴링할 필요가 없다.


### 함수

한정자

1. 함수의 스코프 범위

- public : 외부에서 함수에 직접 접근 가능.
- internal : default, 현재 계약 및 그것을 상속한 계약에서만 사용 가능하다. 외부에서는 이러한 함수들에 접근할 수 없으므로 계약 인터페이스에 속하지 않는다.
- private : 그것을 선언한 계약에서만 사용할 수 있다. 파생 계약에서도 사용할 수 없으며, 계약 인터페이스에 속하지 않는다.
- external : 외부에서 직접적으로 접근할 수 있지만, 내부에서는 접근할 수 없다. 이러한 함수는 계약 인터페이스로 사용된다. (뭐지?)


2. 계약 상태 변수를 변경하는 능력 + 관련한 동작을 바꾸는 능력

- constant : 이 함수는 블록체인의 상태를 수정할 수 없다. 상태 변수를 읽고 호출자에게 반환하는 일은 할 수 있지만, 변수를 수정하거나 이벤트를 호출하거나 다른 계약을 생성하거나 상태를 변경하는 다른 함수를 호출하는 일은 하지 못한다. constant 함수는 현재 상태 변수를 읽고 반환할 수 있는 함수라고 생각하면 된다. (MVC 패턴에서 V만 제공하는 함수 한정자인 듯.)
- view : 뷰 함수는 constant 함수의 별칭이다.
- pure : 순수 함수는 함수의 능력에 제약이 더 많다. 순수 함수는 읽고 쓰지 못한다. 즉 상태 변수에 접근 할 수 없다. 어떤 함수가 현재 상태와 거래 변수에 접근할 수 없게 하려면 순수 함수로 선언하면 된다.
- payable : 함수에 payable 키워드가 붙으면 호출자로부터 이더를 받을 수 있다. 송금자가 이더를 제공하지 않으면 호출이 실패한다. 함수가 payable로 표시되면 이더만 받을 수 있다.


# 자료형

- 값
- 참조

javascript와 비슷한건가, c의 포인터(or c++의 &변수)와 비슷한건가?


솔리디티가 제공하는 값 타입.

- bool
- uint
- int
- address
- byte
- enum


값을 저장하는 것은 메모리의 한 공간을 할당하여 값이 저장된다.
참조는 그 메모리 주소를 저장한다.

이는 c의 포인터와 비슷한 듯 하다. (즉, java가 배열을 저장하거나, javascript가 배열 or 객체 등을 저장하는 방식.)


### 스토리지 및 메모리 데이터 위치

EVM은 변수를 저장하는 용도로 다음 네 가지 데이터 구조를 제공한다.

- 스토리지 : disk와 비슷한 듯. (반영구적 저장.)
- 메모리 : 휘발성
- 호출 데이터 : 모든 입력 함수의 인자와 실행 데이터를 저장하는 곳이다. 이것은 변경할 수 없는 메모리 위치다. (컴퓨터 stack의 data에 해당하는 부분인건가?)
- 스택 : EVM은 변수의 적재와 이더리움 인스트럭션 세트를 가지고 작업하는 중간값을 저장하기 위해 스택을 유지한다. 이것이 EVM의 작업 세트 메모리다. EVM의 스택의 깊이는 1,024단계이며, 그 이상을 저장하면 예외가 발생한다. (overflow 방지인 듯. dfs를 사용할 때 고려하던 것과 비슷한 듯 하다.)


할당 및 데이터 위치는 다음의 규칙으로 설명할 수 있다.

1. 규칙 1

    상태 변수로서 선언되는 변수들은 항상 스토리지 데이터 위치에 저장된다.

2. 규칙 2

    함수 파라미터로서 선언되는 변수들은 항상 메모리 데이터 위치에 저장된다.

3. 규칙 3

    함수 내에서 선언되는 변수들은 기본적으로 메모리 데이터 위치에 저장된다. 그러나 몇 가지 주의할 점이 있다.

        - 참조 타입 변수의 위치는 스토리지가 기본이지만, 값 타입 변수의 위치는 함수 내의 메모리다.
        - 함수 내에서 선언되는 참조 타입 변수의 위치는 스토리지가 기본값이라는 점에 유의하자. 그렇지만 그것은 오버라이드할 수 있다.
        - 참조 타입 변수의 기본 위치를 무시하고 메모리 데이터 위치에 위치시킬 수 있다. 참조형에는 배열, 구조체, 문자열이 있다.
        - 오버라이드 되지 않고, 함수 내에서 선언된 참조형은 항상 상태 변수를 가진다.
        - 함수에서 선언된 값 타입 변수는 오버라이드 되지 못하며, 스토리지 위치에 저장할 수 없다.
        - 매핑은 항상 스토리지 위치에 선언된다. 이는 함수 내에서 선언될 수 없음을 의미한다. 매핑을 메모리형으로 선언할 수는 없지만, 함수 내의 매핑은 상태 변수로 선언된 매핑을 참조할 수 있다.

4. 규칙 4

    호출자가 함수 파라미터에 제공한 인자는 항상 호출 데이터 위치에 저장된다.

5. 규칙 5

    다른 상태 변수로부터 상태 변수에 할당할 때는 항상 새로운 사본을 생성한다. 아래 예에서는 두 개의 값 타입 상태 변수인 stateVar1과 stateVar2를 선언한다. getUInt 함수에서 stateVar2를 stateVar1에 할당한다. 이 단계에서 두 변수의 값은 40이다. 그 다음에 stateVar2의 값을 50으로 변경하고, stateVar1을 반환한다. 반환되는 값은 40으로, 각 변수가 독립적으로 값을 유지함을 알 수 있다.

    (다른 언어들과 비슷하다.)

    ``` solidity
    pragma solidity ^0.4.19;

    contract DemoStorageToStorageValueTypeAssignment {
        uint stateVar1 = 20;
        uint stateVar2 = 40;

        function getUInt() returns (uint) {
            stateVar1 = stateVar2;
            stateVar2 = 50;
            return stateVar1;
        }
    }
    ```

    다음 예에서는 배열 타입 상태 변수 stateArray1과 stateArray2를 선언한다. getUInt 함수에서 stateArray2를 stateArray1에 할당한다. 이 단계에서는 두 변수의 값이 같다. 그 다음 행에서 stateArray2의 값 중 하나를 5로 변경하고 stateArray1 배열에서 같은 위치의 원소를 반환한다. 반환되는 값은 4이므로 각 변수가 독립적인 값을 유지할 수 있다.

    어? 이 부분은 javascript, java, python등의 언어들과 완전히 다르다.

    배열을 그 배열이 시작하는 주소를 저장하여 사용하는 것과는 달리, 솔리디티는 배열 마다 고유의 스토리지 공간을 가지는 것 같다. 이렇게 하면, 퍼포먼스적인 (공간 복잡도)에서는 손해를 보겠지만, 값에 대한 고유성을 가질 수 있는 장점이 있다. 솔리디티 언어의 존재 목적은 결국 원장의 무결적 저장이기 때문에, 퍼포먼스를 조금 잃더라도 그 목적을 더 중요시 여긴 것 같다.

    ``` solidity
    pragma solidity ^0.4.19;

    contract DemoStorageToStorageReferenceTypeAssignment {
        uint[2] stateArray1 = [uint(1), 2];
        uint[2] stateArray2 = [uint(3), 4];

        function getUInt() returns (uint) {
            stateArray1 = stateArray2;
            stateArray2[1] = t;
            return stateArray1[1];  // 4를 반환
        }
    }
    ```

6. 규칙 6

    다른 메모리 변수로부터 스토리지 변수에 할당할 때는 항상 새로운 사본이 생성된다.

    uint 고정 배열 stateArray가 상태 변수로서 선언된다. getUInt 함수에서 로컬 메모리에 위치하는 uint 고정 배열 localArray가 정의 및 초기화된다. 그 다음 행에서는 localArray를 stateArray에 할당한다. 이 단계에서는 두 변수의 값이 같다. 그 다음 행에서 localArray의 원소 중 하나의 값을 10으로 변경하고 stateArray1 배열에서 같은 위치의 원소를 반환한다. 반환되는 값은 2로, 각 변수가 독립적인 값을 유지하는 것을 볼 수 있다.

    ``` solidity
    pragma solidity ^0.4.19;

    contract DemoMemoryToStorageReferenceTypeAssignment {
        uint[2] stateArray;

        function getUInt() returns (uint) {
            uint[2] memory localArray = [uint(1), 2];
            stateArray = localArray;
            localArray[1] = 10;
            return stateArray[1];   // 2를 반환
        }
    }
    ```
    
    규칙 5에서와 마찬가지의 이유로 이렇게 하는 듯?

    값 타입의 상태 변수 stateVar를 선언하고 값을 20으로 초기화한다. getUInt 함수 내에서 로컬 변수 localVar를 값 40으로 선언한다. 그 다음 행에서 로컬 변수 localVar를 stateVar에 할당한다. 이 단계에서 두 변수의 값은 40이다. localVar의 값을 50으로 변경한 다음, stateVar를 반환한다. 반환되는 값은 40으로, 각 변수가 독립적인 값을 유지하는 것을 볼 수 있다.

    ``` solidity
    pragma solidity ^0.4.19;

    contract DemoMemoryToStorageValueTypeAssignment {
        uint stateVar = 20;

        function getUInt() returns (uint) {
            uint localVar = 40;
            stateVar = localVar;
            localVar = 50;
            return stateVar; // 40을 반환
        }
    }
    ```

    이 건 다른 언어와 비슷하다. 이렇게 되는 이유는 storage에 저장하기 때문.

7. 규칙 7

    다른 상태 변수로부터 메모리 변술늘 할당할 때는 항상 새로운 사본이 만들어진다. 아래 예제 코드에서 값 유형의 상태 변수 stateVar가 선언되고 그 값은 20으로 초기화된다. getUInt 함수 내에 uint 형의 지역 변수가 선언되고 그 값은 40으로 초기화된다. stateVar 변수가 localVar 변수에 할당된다. 이 시점에 두 변수의 값은 20이다. 그 후 stateVar의 값은 50으로 변경되고 localVar가 반환된다. 반환되는 값은 20으로, 각 변수가 독립적으로 값을 유지하는 것을 보여준다.

    ``` solidity
    pragma solidity ^0.4.19;

    contracct DemoStorageToMemoryValueTypeAssignment {
        uint stateVar = 20;
        function getUInt() returns (uint) {
            uint localVar = 40;
            localVar = stateVar;
            stateVar = 50;
            return localVar; // 20을 반환
        }
    }
    ```

    규칙 6과 같은 원리. 단지 거꾸로 됐을 뿐이다.

    uint 고정 배열 stateArray가 상태 변수로 선언된다. getUInt 함수 내에서 로컬 메모리에 위치한 uint 고정 배열 localArray가 정의되고 stateArray 변수로 초기화된다. 이 단계에서 두 변수의 값은 같다. 그 다음 행에서 stateArray의 한 원소의 값을 5로 변경하고 localArray1 배열의 같은 위치에 있는 원소의 값을 반환한다. 반환되는 값은 2로, 각 변수가 독립적인 값을 유지하는 것을 보여준다.

    ```solidity
    pragma solidity ^0.4.19;

    contract DemoStorageToMemoryReferenceTypeAssignment {
        uint[2] stateArray = [uint(1), 2];

        function getUInt() returns (uint) {
            uint[2] memory localArray = stateArray;
            stateArray[1] = 5;
            return localArray[1];   // 2를 반환
        }
    }
    ```

    위와 동일.

8. 규칙 8

    다른 메모리 변수로부터 메모리 변수에 할당할 때는 참조 타입에 대한 사본을 생성하지 않고, 값 타입에 대한 새로운 사본을 생성한다. 다음 예제 코드는 메모리 내의 값 타입 변수가 값에 의해 복사되는 것을 보여준다. localVar1의 값은 localVar2 변수의 값이 변경되는 것에 영향을 받지 않는다.

    ``` solidity
    pragma solidity ^0.4.19;

    contract DemoMemoryToMemoryValueTypeAssignment {
        function getUInt() returns (uint) {
            uint localVar1 = 40;
            uint localVar2 = 80;
            localVar1 = localVar2;
            localVar2 = 100;
            return localVal1;   // 80을 반환
        }
    }
    ```

    다른 언어들과 동일.

    다음 예제 코드는 메모리 내의 참조 타입 변수가 참조에 의해 복사되는 것을 보여준다. otherVar의 값은 someVar 변수가 변경되는 것에 영향을 받는다.

    ``` solidity
    pragma solidity ^0.4.19;

    contract DemoMemoryToMemoryReferenceTypeAssignment {
        uint stateVar = 20;

        function getUInt() returns (uint) {
            uint[] memory someVar = new uint[](1);
            someVar[0] = 23;
            uint[] memory otherVar = someVar;
            someVar[0] = 45;
            return (otherVar[0])    // 45를 반환
        }
    }
    ```

    이 경우는 참조형으로 돌아간다. java, python, javascript의 배열과 유사한 방식으로 돌아감.

### 리터럴

리터럴은 프로그램의 시작부터 끝까지 동일한 값을 유지함.

종류

- 1, 10, 100.... (정수)
- "Ritesh", "modi" (문자열, 작은 따옴표도 가능)
- 0xca35d39gffnbfko34385~~~ (address)
- 16진수 리터럴에는 hex 키워드가 붙음. 예) hex"1A2B3F"
- 4.5, 0.2...

### 정수

부호 있는 정수 : 음수 또는 양수 값을 가질 수 있음.
부호 없는 정수 : 0 또는 양의 값을 가질 수 있음.

c와 비슷하다. +, -표현을 위해 써야하는 1비트의 낭비를 줄이는 방법이다. 요즘의 device에는 사실상 공간 복잡도가 큰 의미가 없어졌지만, 이더리움의 실사용을 위해서는 굉장히 작은 공간, 시간을 절약해야 하기 때문에 고려를 하는 것 같다.

``` solidity
pragma solidity ^0.4.19;

contract AllAboutInts {
    uint stateUInt = 20;
    uint stateInt = 20;

    function getUInt(uint incomingValue) {
        uint memoryuint = 256;
        uint256 memoryuint256 = 256;
        uint8 memoryuint8 = 8;

        // 두 개의 uint8을 더함
        uint256 result = memoryuint8 + memoryuint8;

        // assignAfterIncrement = 9이고 memoryuint8 = 9
        uint256 assignAfterIncrement = ++memoryuint8;

        // assignBeforeIncrement = 9이고 memoryuint8 = 10
        uint256 assignBeforeIncrement = memoryuint8++;
    }

    function getInt(int incomingValue) {
        int memoryInt = 256;
        int256 memoryInt256 = 256;
        int8 memoryInt8 = 8;
    }
}
```

### 불린

다른 프로그래밍 언어와 유사, but 다른 프로그래밍 언어와는 달리 솔리디티의 불린은 정수로 변환되지 않음.

bool의 기본값은 false.

``` solidity
pragma solidity ^0.4.19;

contract BoolContract {
    bool isPaid = true;

    function manageBool() returns (bool) {
        isPaid = false;
        return isPaid;  // false를 반환
    }

    function convertToUint() returns (uint8) {
        isPaid = false;
        return uint8(isPaid);   // 오류
    }
}
```

### 바이트 자료형

바이트란 8비트의 부호 있는 정수를 가리킨다.
bytes1 자료형은 1바이트이고, bytes2 자료형은 2바이트이다.
솔리디티에서 byte형은 bytes1에 대한 별칭이다.

``` solidity
pragma solidity ^0.4.19;

contract BytesContract {
    bytes1 aa = 0x65;
    bytes1 bb = 10;
    bytes2 cc = 256;
    bytes1 dd = 'a';
    bytes1 ee = -100;

    function getintaa() returns (uint) {
        return uint(aa);    // 101을 반환
    }

    function getbyteaa() returns (bytes1) {
        return aa;          // 0x65를 반환
    }

    function getbytebb() returns (bytes1) {
        return bb;  // 0x0a를 반환
    }

    function getintbb() returns (uint) {
        return uint(bb);    // 10을 반환
    }

    function getbytecc() returns (bytes2) {
        return cc;  // 0x0100을 반환
    }

    function getintcc() returns (uint) {
        return uint(cc);    // 256을 반환
    }

    function getbytedd() returns (bytes2) {
        return dd;      // 0x6100 또는 bytes1에 대해 0x61을 반환
    }

    function getintdd() returns (uint) {
        return uint(dd);    // 97을 반환
    }
}
```

### 배열

솔리디티에서 배열은 고정 배열과 동적 배열을 사용할 수 있다.

1. 고정 배열

선언할 때 크기가 정해진 것.

예.
int[5] age;
byte[4] flags;

고정 배열은 new 키워드로 초기화할 수 없고, 아래 코드와 같이 인라인으로만 초기화할 수 있다.

int[5] age = [int(10), 20, 30, 40, 50];

또한 다음과 같이 함수 내에서 인라인으로 초기화할 수도 있다.

int[5] age;
age = [int(10), 2, 3, 4, 5];

2. 동적 배열

동적 배열이란 그 크기가 선언 시에 확정되는 것이 아니라 실행 시간에 결정되는 배열을 가리킨다.

int[] age;
bytes[] flags;

동적 배열은 인라인으로 초기화할 수도 있고 new 연산자를 사용해 초기ㅗ하할 수도 있다. 다음과 같이 선언하는 시점에 초기화할 수 있다.

int[] age = [int(10), 20, 30, 40, 50];
int[] age = new int[](5);

다음과 같이 두 단계로 나눠서 나중에 함수에서 초기화할 수도 있다.

int[] age;
age = new int[](5);