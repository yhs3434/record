# solidity

1. data type
2. reference type
3. mapping

# data type

- boolean : true / false
``` solidity
bool public b = false;

bool public b1 = !false;
bool public b2 = false || ture;
bool public b3 = false == true;
bool public b4= false && true;
```

- bytes

    ``` solidity
    bytes4 public bt = 0x12345678;
    bytes public bt2 = "STRING";
    ```

- address

``` solidity
address public addr = 0x41241231241212412412412412412455;

```
    
- uint

``` solidity
int8 : -2^7 ~ 2&7 - 1
uint8 : 0 ~ 2^8 - 1

int8 public it = 4;
uint256 public uit = 132321;
```

