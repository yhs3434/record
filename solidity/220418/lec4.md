``` solidity
uint256 public a = 3;
```

function 선언

1. Parameter는 없고 return 값도 없는 function.
``` solidity
function changeA1() public {
    a = 5;
}
```

2. parameter는 있고, return 값은 없는 function.
``` solidity
function changeA2(uint256 _value) public {
    a = _value;
}
```

3. parameter는 있고, return 값도 있는 function.
``` solidity
function changeA3(uint256 _value) public returns (uint256) {
    a = _value;
    return a;
}
```