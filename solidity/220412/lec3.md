1 ehter = 10^9 Gwei = 10^18 wei
0.01 ether = 10^16 wei

Gwei = 가스를 지불할 때의 단위.

``` solidity
uint256 public value = 1 ether;
uint256 public value2 = 1 wei;
uint256 public value3 = 1 gwei;
```

이더리움 컨트랙트와의 모든 통신은 gas를 소모한다.

이유 : 무제한 트랜잭션을 막기 위해.