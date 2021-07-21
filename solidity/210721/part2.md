사설 네트워크는 genesis.json 을 initialize하여 geth를 실행하면 사용할 수 있다.

그게 아니라면,

chainId

1 : mainnet,
3 : ropsten,
4 ~~~


BSC, HECO, CELO, ORBIT 등등 이런 사설 네트워크들은 genesis.json에서 설정한 네트워크인걸까?

ganache-cli로 실행했을 때, rpc 주소는 http://localhost:8545가 된다.
(ganache-cli는 편리하게 공부할 수 있도록, 가상의 사설네트워크를 만들어준다.)


ganache-cli로 로컬 네트워크 실행,

metamask로 localhost:8545에 연결.

ganache-cli로 생성된 지갑을 import.