### var 타입 변수

묵시적으로 타입이 정해지는 변수이다.
var 변수의 최종적인 자료형은 개발자가 정하는 것이 아니라 컴파일러가 정한다.

``` solidity
pragma solidity ^0.4.19;

contract VarExample {
    function VarType() {
        var uintVar8 = 10;  // uint8
        uintVar8 = 255; // 256은 오류

        var uintVar16 = 256;    // uint16
        uintVar16 = 65535;  // 오류

        var intVar8 = -1;   // int8
        var intVar16 - -129 // int16

        var boolVar = true;
        boolVar = false;

        var stringVar = "0x10"; // 문자열 메모리
        stringVar = "10";   // 오류

        var bytesVar = 0x100;   // 바이트 메모리

        var Var = hex"001122FF";

        var arrayInteger = [uint8(1), 2];
        arrayInteger[1] = 255;

        var arrayByte = bytes10(0x2222);
        arrayByte = 0x1111111111111 // 오류
    }
}
```

### 변수 호이스팅

솔리디티의 컴파일러는 함수의 어디에서나 변수를 선언하면, 해당 변수를 함수 시작 부분에 배치한다.

``` solidity
pragma solidity ^0.4.19;

contract variableHoisting {
    function hoistingDemo() returns (uint) {
        firstVar = 10;
        secondVar = 20;

        result = firstVar + secondVar;

        uint firstVar;
        uint secondVar;
        uint result;
        return result;
    }
}
```

### 변수 스코프

솔리디티에서 변수를 선언할 수 있는 위치는 두 군데가 있다.

1. 계약 수준 전역 변수(상태 변수)
2. 함수 수준 지역 변수

함수 수준 지역 변수는 함수 내부의 어디서든 사용할 수 있고, 함수 밖에서는 사용할 수 없다.

계약 수준 전역 변수는 계약 내의 생성자, 폴백, 수정자를 포함한 모든 함수에서 사용할 수 있는 변수다. 계약 수준 전역 변수는 가시성 수정자를 곁들일 수 있다. 상태 데이터를 가시성 수정자와 관계없이 전체 네트워크에서 조회할 수 있음을 이해하는 것이 중요하다. 다음의 상태 변수는 함수를 사용해야만 수정할 수 있다.

- public : 이 상태 변수들은 외부 호출에서 직접 접근할 수 있다. 컴파일러는 public 상태 변수의 값을 읽는 데 사용할 getter 함수를 묵시적으로 생성한다.
- internal : 이 상태 변수는 외부 호출에서 직접 접근할 수 없다. 현재 계약과 그로부터 파생된 지식 계약의 함수에서 접근할 수 있다.
- private : 이 상태 변수는 외부 호출에서 직접 접근할 수 없으며, 자식 계약의 함수에서도 접근할 수 없다. 현재 계약의 함수에서만 접근할 수 있다.


### 형 변환

솔리디티가 적정 형 언어를 컴파일 시간에 특정 자료형으로 정의되는 것으로 알고있다. 따라서 변수의 자료형은 변경할 수 없다.

``` solidity
pragma solidity ^0.4.19;

contract ErrorDataType {
    function hoistingDemo() returns (uint) {
        uint8 someVar = 100;
        someVar = 300;  // 오류
    }
}
```

그렇지만 값을 복사하면서 그것의 타입을 바꾸고자 할 때는 변환을 해야하는데, 이것을 형 변환이라 한다.

- 묵시적 변환

묵시적 변환이란 연산자를 사용하거나 외부의 도움을 받지 않고도 변환이 되는 것이다. 예를 들어, uint8은 uint16으로 묵시적으로 변환된다.

- 명시적 변환

데이터 손실이 우려되거나 데이터가 목표 데이터 타입 범위에 맞아떨어지지 않는 등의 이유로 컴파일러가 묵시적 변환을 수행하지 않을 때는 명시적 변환이 요구된다. 예컨대 uint16을 uint8로 변환하려면 명시적 변환이 필요하며, 그 과정에서 데이터가 손실될 가능성이 있다.

다음 코드에서 묵시적 변환과 명시적 변환의 예를 볼 수 있다.

``` solidity
pragma solidity ^0.4.19;

contract ConversionDemo {
    function ConversionExplicitUINT8toUINT256() returns (uint) {
        uint8 myVariable = 10;
        uint256 someVariable = myVariable;
        return someVariable;
    }
    
    function ConversionExplicitUINT256toUINT8() returns (uint8) {
        uint256 myVariable = 10;
        uint8 someVariable = uint8(myVariable);
        return someVariable;
    }
    // uint256 => uint8 로 명시적 변환.


    function ConversionExplicitUINT256toUINT81() returns (uint8) {
        uint256 myVariable = 10000134;
        uint8 someVariable = uint8(myVariable);
        return someVariable;    // 6을 반환
    }
    // 데이터 유실

    function Conversions() {
        uint256 myVariable = 10000134;
        uint8 someVariable = 100;
        bytes4 byte4 = 0x65666768;

        // bytes1 byte1 = 0x656667668;  // 오류
        bytes1 byte1 = 0x65;

        // byte1 = byte4;
        byte1= bytes1(byte4);   // 명시적 변환

        byte4 = byte1;  // 묵시적 변환

        // uint8 someVariable = myVariable; // 오류, 명시적으로 변환해야 함

        myVariable = someVariable;  // 묵시적 변환

        string memory name = "Ritesh";
        bytes memory nameInBytes = bytes(name); // 문자열에서 바이트로 명시적 변환

        name = string(nameInBytes); // 바이트에서 문자열로 명시적 변환
    }
}
```


### 블록 및 거래 글로벌 변수

솔리디티에는 계약 내에서 선언되지 않았지만 계약 내의 코드에서 접근할 수 있는 몇 가지 전역 변수가 있다. 기본적으로 계약은 원장에 직접적으로 접근할 수 없다. 원장은 채굴자에 의해서만 관리된다. 그러나 솔리디티에서 계약에 대한 현재 거래와 블록에 대한 몇 가지 정보를 제공하므로 그것을 이용할 수 있다.

다음 코드는 전역 거래, 블록, 메시지 변수를 사용하는 예를 보여준다.

``` solidity
pragma solidity ^0.4.19;

contract TransactionAndMessageVariables {
    event logstring(string);
    event loguint(uint);
    event logbytes(bytes);
    event logaddress(address);
    event logbytes4(bytes4);
    event logblock(bytes32);

    function globalVariable() payable {
        logaddress(block.coinbase);
        loguint(block.difficulty);
        loguint(block.gaslimit);
        loguint(msg.gas);
        loguint(tx.gasprice);
        loguint(block.number);
        loguint(block.timestamp);
        loguint(now);
        logbytes(msg.data);
        logbyte4(msg.sig);
        loguint(msg.value);
        logaddress(msg.sender);
        logaddress(tx.origin);
        logblock(block.blockhash(block.number));
    }
}
```