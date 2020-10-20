# 2020년 10월 20일

## 학습 목록

- macOS BrowserWindows에 대한 파일 표현

macOS는 창에서 대표 파일을 설정할 수 있다. 타이틀바에 파일 아이콘이 있고, 사용자가 Command - Click 또는 Control - Click 키를 누를 때 파일 경로 팝업이 보여진다.

또한 창의 상태도 지정할 수 있다. 다시 말 해 로드된 문서의 수정 여부를 제목의 파일 아이콘에 표시할 수 있다.

라고 하는데 mac 쓰면 그런가 보다. 난 윈도우랑 리눅스만 써봐서 잘 모르겠음 ㅠ 어쨌든 BrowserWindow 객체의 setRepresentedFilename이나 setDocumentEdited를 사용하면 된다고 한다.

예)

```javascript
const { BrowserWindow } = require('electron');

const win = new BrowserWindow();
win.setRepresentedFilename('/etc/passwd');
win.setDocumentEdited(true);
```


- 기본 파일 드래그 & 드랍

파일을 조작하는 애플리케이션을 만든다면 네이티브의 file drag & drop 기능이 지원되기를 몹시 원할 것이다. 예를 들어서 크롬같은 경우에 pdf파일을 드래그앤 드랍하면 pdf가 켜지듯. Electron은 웹 콘텐츠에서 운영 체제의 세계로 파일 및 콘텐츠를 드래그하는 기능을 추가적으로 지원한다. 웹 -> 운영체제 하는 법.

예)

   - index.html
```html
<a href="#" id="drag">Drag me</a>
<script src="rendered.js"></script>
```

   - rendered.js
```javascript
const { ipcRendered } = require('electron');

document.getElementById('drag').ondragstart = (event) => {
    event.preventDefault();
    ipcRendered.send('ondragstart', '/absolute/path/to/the/item');
};
```

   - main.js
```javascript
const { ipcMain } = require('electron');

ipcMain.on('ondragstart', (event, filePath) => {
    event.sender.startDrag({
        file: filePath,
        icon: '/path/to/icon.png'
    });
});
```


- 오프 스크린 렌더링

오프 스크린 렌더링을 사용하면 브라우저 창의 내용을 비트 맵으로 가져올 수 있고, 그 이미지를 3D scene의 텍스처와 같이 어디에서나 렌더링 할 수 있다고 한다. Electron의 오프 스크린 렌더링은 Chromium 임베디드 프레임워크 프로젝트와 비슷한 방식을 사용한다.

신기한 기능인 것 같은데 오픈GL에서 처음 쓴 것 같다? 대충 검색해보니 그런듯.

기본적으로 성능 좋은 렌더링을 위해 GPU 가속을 사용하는데, 만약 CPU가 소프트웨어로 렌더링을 처리하기 위해서는 app.disableHardwareAcceleration() 을 사용하면 된다.

```javascript
const { app, BrowserWindow } = require('electron');

app.disableHardwareAcceleration();

let win;

app.whenReady().then(() => {
    win = new BrowserWindow({
        webPreferences: {
            offscreen: true
        }
    });

    win.loadURL('http://github.com');
    win.webContents.on('paint', (event, dirty, image) => {
        // updateBitmap(dirty, image.getBitmap());
    })
    win.webContents.setFrameRate(30);
})
```


- macOS 다크 모드 지원

맥OS 는 10.14 부터 모든 컴퓨터에 대해 시스템 차원의 다크 모드를 지원했고, 이 기능을 사용하는 방법이다.

```javascript
const { nativeTheme } = require('electron');

nativeTheme.on('updated', function theThemeHasChaged () {
    updateMyAppTheme(nativeTheme.shouldUseDarkColors);
});
```


- Electron에서의 웹 삽입

만약 내가 외부의 컨텐츠(서드파티)를 일렉트론의 BrowserWindow에 삽입하기를 원한다면 3가지 방법이 있다. Iframe, WebView, BrowserView 이렇게 3가지 방법. 각 방법은 장단점이 있고 특징이 있기때문에 설명함.

   + Iframe

   일렉트론의 iframe은 일반적인 다른 브라우저의 iframe과 유사하다. 따라서 보안적으로 좋은 태그가 아니기때문에 만약 사용한다면 sandbox 영역을 만들어서 사용하는 것이 낫다.

   + WebViews

   현재는 크로미움의 웹뷰를 사용하고 있다. 따라서 현재의 WebView API가 Electron의 미래 버전에서도 계속 사용 가능할 것이라고 보장되지 않는다. 만약 그래도 사용하기를 원한다면 BrowserWindow의 webPreferences에서 webviewTag를 true로 설정하라.

   웹뷰 태그와의 모든 커뮤니케이션은 비동기적으로 이루어진다. 그리고 웹뷰는 아무래도 크로미움에서 만들었기에 더 많은 기능을 가지고 있을 것이라고 본다.

   iframe태그에 비해서 웹뷰 태그가 다소 느린 경향은 있지만, 다양한 이벤트 처리에 있어서 더 큰 통제력을 지원할 것이다.

   + BrowserViews

   브라우저뷰는 DOM의 한 부분이 아니다.대신에 브라우저뷰는 메인 프로세스 위에서 동작한다. 이 말은 브라우저 뷰가 완벽하게 DOM또는 CSS와 분리되어 있음을 의미하며 또한 BrowserWindow 컨텐츠와 완전히 분리되어 있음을 의미한다.

   브라우저뷰를 이용하면 컨텐츠를 nodejs로 관리할 수 있는 장점이 있지만, 내 DOM의 일부가 아니기때문에 브라우저 뷰의 위치를 수동으로 관리해야 할 것이다.

- 접근성

   + Spectron

   ```javascript
   app.client.auditAccessibility().then((audit) => {
       if (audit.failed) {
           console.error(audit.message);
       }
   })

   + Devtron

   크롬을 위해 구글이 개발한 접근성 개발자 도구 라이브러리를 통해 이 도구를 이용할 수 있다.