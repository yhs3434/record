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
