## link

Adding 2D content to a WebGL context (https://developer.mozilla.org/ko/docs/Web/API/WebGL_API/Tutorial/Adding_2D_content_to_a_WebGL_context#%EA%B0%9D%EC%B2%B4_%EC%83%9D%EC%84%B1)

~

## 드는 생각

1. 아 어렵다. 그래픽 처리 관련 서적을 보아야 할 것 같다.
2. 내가 하고싶은 것을 찾은 것 같다.

## 행렬 유틸리티를 이용한 연산

벡터와 행렬 연산을 자바스크립트에서 다루는 데 아주 편리한 라이브러리. Sylvester(http://sylvester.jcoglan.com/).



# what is WebGL?

WebGL은 Web Graphics Library의 약자로 웹상에서 2D 및 3D 그래픽을 렌더링하기 위한 로우 레벨 Javascript API이다. OpenGL ES 2.0을 기반으로 브라우저 엔진에 내장된 HTML5 Canvas 요소 위에 그려진다.

사실 canvas는 처음부터 3D 그래픽을 렌더링하진 못했다. 처음에는 일부 위젯과 애플 운영체제의 애플리케이션에서 2D 그래픽 렌더링을 위해 공개되었었다. 추후에 WebGL이 등장 이후부터 3D 게임이나 고급 3D 그래픽 애플리케이션을 만들 수 있게 됐다.

## WebGL의 장점

- 로열티 없이 누구나 사용 가능하다.
- 렌더링 가속화를 지원하는 그래픽 하드웨어를 활용한다.
- 별도의 플러그인이 필요 없으며, 웹 브라우저에 내장되어 실행된다.
- OpenGL ES 2.0을 기반으로 하므로, 이미 OpenGL API에 대한 경험이 있다면 다루기가 쉽다.
- 자바스크립트 프로그래밍이 가능하다. 자바스크립트는 자동 메모리 관리를 지원하기 때문에 메모리를 수동으로 할당할 필요도 없고 WebGL이 자바스크립트의 기능을 상속 받는다.
- 모바일 브라우저에서도 사용 가능하다(모든 모바일 브라우저를 의미하진 않습니다.)

## 렌더링

위에서 WebGL은 2D, 3D 그래픽을 "렌더링"한다고 말했다. 여기서 잠깐 렌더링에 대해 살펴보자. 렌더링은 컴퓨터 프로그램을 사용하여 모델에서 이미지를 생성하는 프로세스이다. 이런 렌더링 유형에는 두 가지가 있다.

- 소프트웨어 렌더링 : 모든 그래픽 계산을 CPU의 도움으로 수행한다.
- 하드웨어 렌더링 : 모든 그래픽 계산을 GPU에 의해 수행한다.

그리고 렌더링은 **서버 사이드 렌더링**을 통해 리소스가 충분한 전용 서버에서 할 수도 있고, **클라이언트 사이드 렌더링**을 통해 CPU에서 로컬로 수행할 수도 있다. 우리가 앞으로 만나게 될 WebGL은 클라이언트 사이드 렌더링을 통해 3D 장면을 렌더링하며, 이미지를 얻는데 필요한 모든 처리는 클라이언트의 그래픽 하드웨어를 사용해서 로컬에서 수행하게된다.

## WebGL2도 있다는데?

그렇다 존재한다. OpenGL ES 3.0을 기반으로 하고 있으며 멋진 새로운 기능들이 추가되었다. 다만 2.0을 기준으로 아래 글을 작성하진 않을 건데, 이유는 아직 사파리에 대한 지원이 완벽하게 되지 않아서 이다.

웹에서 그래픽 프로그램을 만들기 위해서는 HTML5의 canvas 요소를 사용한다. WebGL에 대해 익히기 전에 canvas를 먼저 보자.

## HTML5 Canvas

canvas는 자바스크립트를 이용해서 그래픽을 그릴 수 있는 쉽고 강력한 여러 옵션을 제공한다.

``` html
<canvas id="excanvas" width="300" height="150"></canvas>
```

캔버스에 WebGL 렌더링 컨텍스트를 생성하려면 캔버스로부터 컨텍스트를 얻는 메소드인 canvas.getContext() 메서드에 'experiment-webgl'을 매개변수로 넘겨준다.

``` javascript
const canvas = document.getElementById('excanvas');
const ctx = canvas.getContext('experimental-webgl');
```

## WebGL 기초 - 좌표 시스템

WebGL은 대부분 3D API가 아닌 저서준의 래스터 API이다. (그래서 기본 개념을 숙지 후 뒷부분에서는 three.js를 이용해서 3D 그래픽을 만들어 볼 예정이다). 그래서 우리는 WebGL 애플리케이션을 만들기 위해서 먼저 WebGL의 좌표 시스템을 살펴볼 것이다.

(그림 있음)

여기서 x, y축은 아마도 익숙할 것이다. 우린 여기에 z축만 더 추가해서 기억하자. z축은 깊이를 나타낸다. 양수 z값은 화면과 가까움을 의마하며, 음수 z 값은 화면에서 멀리 있음을 의미한다.

## WebGL 기초 - 용어

1. 정점(vertex) - 정점은 3차원 개체의 가장자리의 결합을 형성하는 점이다. 우리는 자바스크립트 배열을 사용하여, 정점을 수동으로 저장하고 정점 버퍼를 사용하여 WebGL 렌더링 파이프 라인에 전달해야 한다.

2. 인덱스(index) - 인덱스는 정점을 식별하는 숫자 값이다. 이 인덱스를 이용하여 WebGL의 메쉬를 그리는데 사용한다.

3. 버퍼(buffer) - 버퍼는 데이터를 가지고 있는 WebGL의 메모리 영역이다. 그리고 이 버퍼 영역에는 다양한 버퍼가 존재한다.

4. 메쉬(mesh) - 2D 또는 3D 개체를 그리기 위해 WebGL API에서 drawArrays(mode)와 drawElements(mode) 메서드를 제공해 준다. mode는 점, 선, 삼각형만 가능하다. 그리고 이 두가지 방법을 이용하여 점, 선, 삼각형으로 하나 이상의 다각형을 구성하며, 이런 기본 다각형을 사용하여 메쉬를 형성한다.

5. 셰이더 프로그램 - WebGL을 GPU 가속 컴퓨팅을 사용하기 때문에(애플리케이션이 cpu에 로드되고 코드에서 계산이 많은 부분을 발견할 때마다 해당 코드 부분이 GPU에서 로드되고 실행되는 방식), 우리가 만든 메쉬 정보를 CPU에서 GPU로 전송해야 되는 많은 통신 오버 헤드가 발생한다. 이 때 WebGL에서는 셰이더 프로그램을 사용하여 클라이언트 시스템에 그래픽 요소를 그리는데 필요한 모든 프로그램을 작성한다.
 셰이더는 GPU용 프로그램이며, 정점, 변형, 재질, 조명 및 카메라가 상호 작용하여 특정 이미지를 만드는 방법을 정확하게 정의한다. 쉽게 말해 메쉬에 대한 픽셀을 가져오는 알고리즘을 구현한 것 이다. 그리고 이 셰이더는 정점 셰이더와 프래그먼트 셰이더의 두 가지 유형이 존재한다.

 6. 정점 셰이더(vertex shader) - 모든 정점에서 호출되는 프로그램 코드이다. 기하학을 한 위치에서 다른 위치로 변환(이동)하는데 사용된다. 개발자는 이 셰이더의 속성을 정의해야 하며, 자바스크립트로 작성된 Vertex Buffer Object를 가리킨다.
  정점 변환, 정상 변환 및 정규화, 텍스처 좌표 생성, 텍스처 좌표 변환, 조명, 색상 재료 적용 등을 할 수 있다.

7. 프래그먼트 셰이더(fragment shader, pixed shader) - 메쉬는 여러 삼각형으로 구성되며 각 삼각형의 표면을 프래그먼트라고 한다. 프래그먼트 셰이더는 모든 프래그먼트의 픽셀에서 실행되는 코드이다. 개별 픽셀의 색상을 계산하고 채우기 위해 사용한다.
 보간된 값에 대한 연산, 텍스처 접근, 텍스처 적용, 안개, 색상 합계 등을 할 수 있다.

> 출처 : https://code-masterjung.tistory.com/110