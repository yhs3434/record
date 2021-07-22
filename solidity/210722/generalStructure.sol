pragma solidity ^0.4.19;

contract GeneralStructure {
    // 상태 변수
    int public stateIntVariable;    // 정수형 변수
    string stateStringVariable; // 문자열 변수
    address personIdentifier;   // 주소형 변수
    myStruct human; // 구조체 변수
    bool constant hasIncome = true; // 상수의 성질을 갖는 변수.

    // 구조체 정의
    struct myStruct {
        string name;
        uint myAge;
        bool isMarried;
        uint[] bankAccountsNumbers;
    }

    // 수정자 선언
    modifier onlyBy() {
        if (msg.sender == personIdentifier) {
            _;
        }
    }

    // 이벤트 선언
    event ageRead(address, int);

    // 열거형 선언
    enum gender {male, female}

    // 함수 선언
    function getAge(address _personIdentifier) onlyBy() payable external returns (uint) {
        human = myStruct("Ritesh", 10, true, new uint[](3));
        gender _gender = gender.male;
        ageRead(personIdentifier, stateIntVariable);
    }
}