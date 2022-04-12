1. parameter 와 return 값이 없는 function

``` solidity
uint256 public a = 3;

function nchangeA1() public {
    a = 5;
}
```

2. parameter는 있고, return 값이 없는 function

``` solidity
function changeA2(uint256 _value) public {
    a = _value;
}
```

3. parameter 는 있고, return 값이 있는 function
``` solidity
function changeA3(uint _value) public returns(uint256) {
    a = _value;
    return a;
}
```