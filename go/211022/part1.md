https://play.golang.org/ 에서 컴파일러 설치 없이 실습 가능.

### 변수 선언 방식.

``` go
var x int
var arr [5]int
func(int, int)
func(int) int
func(int, func(int, int)) func(int) int
var p *int
var x int = 10
```

### 자료형 추론

``` go
var i = 10
var p = &i
i := 10
p := &i
```

``` go
i := 10
s := "hello"
i = 20
j = 30 // x
i = "hello" // x
i := 30 // x
i:= "hi" // x
```

### 함수와 간단한 제어 구조

.