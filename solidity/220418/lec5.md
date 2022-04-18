# 접근 제한자

1. public : 모든 곳에서 접근 가능

``` solidity
contract Public_example {
    Public_example instance = new Public_example();

    function changeA_2 (uint256 _value) public {
        instance.changeA(_value);
    }

    function use_public_example_a() view public returns (uint256) {
        return instance.get_a();
    }
}

contract Public_example_2 {
    uint256 public a = 3;

    function changeA (uint256 _value) public {
        a = _value;
    }

    function get_a() view public returns (uint256) {
        return a;
    }
}
```

2. external : external이 정의된 자기 자신 컨트랙내에서 접근 X, 나머지는 public과 같음.

3. private : 오직 자기 컨트랙안에서만 가능.

4. internal : 계약서 자신과 상속 받은 계약서만 사용할 수 있다.