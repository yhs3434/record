# 2020년 10월 22일

## 학습 목록

- 테스팅과 디버깅

렌더러 프로세스에서는 크롬의 개발자 도구를 이용하는 것이 가장 용이한 방법이다.

```javascript
const { BrowserWindow } = require('electron');

const win = new BrowserWindow();
win.webContents.openDevTools();
```

메인 프로세스에서는 개발자 도구를 열 수 없기 때문에, 주요 프로세스를 디버깅 하는 것이 조금 까다롭다.

- 메인 프로세스 디버깅하기

외부 디버거를 이용해서 디버깅 해야 한다. Electron 실행시 --debug 또는 --debug-brk 스위치가 필요하다.

다음 커맨드 라인 스위치들을 사용하여 메인 프로세스를 디버깅 할 수 있다.

--inspect=[port]

```cmd
electron --inspect=5858 your/app
```

--inspect-brk=[port]

--inspect와 같지만 javascript 첫번째 line에서 실행을 중단한다.


- VS Code에서 메인 프로세스 디버깅하기

1. VS Code에서 Electron 프로젝트 열기

```bash
git clone git&github.com:electron/electron-quick-start.git
code electron-quick-start
```

2. 다음 설정으로 .vscode/launch.json 파일 추가하기.

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug Main Process",
      "type": "node",
      "request": "launch",
      "cwd": "${workspaceFolder}",
      "runtimeExecutable": "${workspaceFolder}/node_modules/.bin/electron",
      "windows": {
        "runtimeExecutable": "${workspaceFolder}/node_modules/.bin/electron.cmd"
      },
      "args" : ["."],
      "outputCapture": "std"
    }
  ]
}
```

3. 디버깅


-  Selenium과 WebDriver 사용하기

WebDriver는 많은 브라우저에서 웹 응용 프로그램을 자동으로 테스트 할 수 있는 오픈 소스 도구이다. 웹 페이지, 사용자 입력, JavaScript 실행 등을 탐색 할 수 있는 기능을 제공한다. ChromeDriver는 Chromium용 WebDriver의 와이어 프로토콜을 구현하는 독립 실행형 서버이다. Chromium 및 WebDriver 팀 구성원이 개발 중이다.

Spectron 설정

Spectron은 공식적으로 지원되는 Electron의 ChromeDriver 테스트 프레임 워크이다. WebdriverIO 위에 구축되어 ChromeDriver테스트 및 번들에서 Electron API에 엑세스 할 수 있는 도우미가 있다.

```
$ npm install --save-dev spectron
```

```javascript
const Application = require('spectron').Application;
const assert = require('assert');

const myApp = new Application({
    path: '/Applications/MyApp.app/Contents/MacOS/MyApp'
});

const verifyWindowIsVisibleWithTitle = async (app) => {
    await app.start();
    try {
        // 윈도우가 보이는지 확인
        const isVisible = await app.browserWindow.isVisible();
        // 윈도우가 보이는지 검증
        assert.strictEqual(isVisible, true);
        // 윈도우의 제목을 가져옴
        const title = await app.client.getTitle();
        // 윈도우의 제목을 검증
        assert.strictEqual(title, 'My App');
    } catch (error) {
        // 실패 로깅
        console.error('Test failed', error.message);
    }
    // 어플리케이션 중단
    await app.stop();
}

verifyWindowIsVisibleWithTitle(myApp);
```

WebDriverJs로 설정하기

WebDriverJs는 웹 드라이버 테스트를 위한 Node 패키지를 제공하며 다음 예제처럼 사용합니다.

1. Start ChromeDriver

먼저, chromedriver 바이너리를 다운로드 받고 실행한다.

```
$ npm install electron-chromedriver
$ ./node_modules/.bin/chromedriver
Starting ChromeDriver (v2.10.291558) on port 9515
Only local connections are allowed.
```
포트 9515는 나중에 사용하므로 기억해 놓는다.

2. Install WebDriverJs

```
$ npm install selenium-webdriver
```

3. Connect to ChromeDriver

selenium-webdriver 를 Electron과 같이 사용하는 방법은 기본적으로 upstream과 같다. 한가지 다른점이 있다면 수동으로 크롬 드라이버 연결에 대해 설정하고 Electron 실행파일의 위치를 전달한다.

```javascript
const webdriver = require('selenium-webdriver');

const driver = new webdriver.Builder()
// Chrome driver에서 "9515" 포트를 사용
    .usingServer('http://localhost:9515')
    .withCapabilities({
        chromeOptions: {
            // 사용할 Electron 바이너리의 경로
            binary: '/Path-to-Your-App.app/Contents/MacOS/Electron'
        }
    })
    .forBrowser('electron')
    .build()

driver.get('http://www.google.com');
driver.findElement(webdriver.By.name('q')).sendKeys('webdriver');
driver.findElement(webdriver.By.name('btnG')).click();
driver.wait(() => {
    return driver.getTitle().then((title) => {
        return title === 'webdriver - Google Search'
    })
}, 1000);

driver.quit();
```


WebdriverIO 설정하기

WebdriverIO는 웹 드라이버와 함께 테스트 하기 위해 제공되는 node 패키지이다.

1. Start ChromeDriver

먼저, 크롬 드라이버 바이너리를 다운받고 실행.

2. Install WebdriverIO

```
$ npm install webdriverio
```

3. Connect to chrome driver

```javascript
const webdriverio = require('webdriverio');
const options = {
    host: 'localhost',
    port: 9515,
    desiredCapabilities: {
        browserName: 'chrome',
        'goog:chromeOptions': {
            binary: '/Path-to-Your-App/electron',
            args: []
        }
    }
};

const client = webdriverio.remote(options);

client.init().url('http://google.com').setValue('#q', 'webdriverio').click('#btnG')
.getTitle().then((title) => {
    console.log('Title was: ' + title)
})
.end();
```


- 헤드리스 CI 시스템 (트래비스, 젠킨스) 테스트

Chromium을 기반으로 하는 Electron은 디스플레이 드라이버 작동을 요구한다. 만약 Chromium이 디스플레이 드라이버를 찾을 수 없는 경우, Electron을 실행하는 데 실패한다. 따라서 실행하는 방법에 관계없이 테스트를 실행하지 않는다. Travis, Circle, Jenkins 또는 유사한 시스템에서 Electron기반 앱을 테스트하려면 약간의 구성이 필요하다. 실제로, 우리는 가상 디스플레이 드라이버의 사용이 ㅣㅍㄹ요하다.


가상 디스플레이 서버 구성

우선 Xvfb를 설치한다. X11 디스플레이 서버 프로토콜을 구현된 가상 프레임 버퍼이며 우리가 정확하게 필요로 하는 기능인 화면에 출력하지 않고 메모리에서 모든 그래픽 작업을 수행한다.

그런 다음 가상 Xvfb 화면을 만들고 이를 가리키는 DISPLAY라는 환경 변수를 내보낸다. Electron의 Chromium은 자동으로 #DISPLAY를 검색하므로, 더이상 앱의 구성이 필요 없다. 이 단계는 xvfb-maybe로 자동화 할 수 있다. xvfb-maybe로 테스트 명령을 추가하면 작은 도구가 현재 시스템에 필요한 경우 Xvfb를 자동으로 구성한다. Windows 또는 macOS에서는 아무 작업도 수행하지 않는다.

```
## Windows 나 macOS에서, 다음 명령은 electron-mocha를 실행한다.
## Linux에서, UI없는 환경이라면
## xvfb-run electron-mocha ./test/*.js 가 동일한 명령이다.
xvfb-maybe electron-mocha ./test/*.js
```


Travis CI

Travis에서 .travis.yml은 대략 다음과 같이 보일 것 입니다.

```
addon:
    apt:
        packages:
            - xvfb

install:
    - export DISPLAY=':99.0'
    - Xvfb :99 -screen 0 1024x768x24 > /dev/null 2>&1 &
```

Jenkins

Jenkins의 경우 Xvfb 플러그인을 사용할 수 있다.

Circle CI

Circle CI는 훌륭하고 Xvfb와 $Display 이미 설정되어 있으므로 추가 구성이 필요하지 않다.

AppVeyor

AppVeyor는 Windows에서 실행되며, Selenium, Chromium, Electron 및 유사 도구를 즉시 지원한다.


- DevTools 확장

Electron은 유명한 웹 프레임워크를 디버깅하기 위해 사용할 수 있는 개발자 도구 확장 기능을 사용할 수 있도록 Chrome DevTools Extension을 지원한다.

문서에서는 확장 기능을 수동으로 로드하는 방법을 설명하지만 electron-devtools-installer와 같은 Chrome WebStore에서 자동으로 확장 기능을 다운로드하는 서드-파티 도구를 사용할 수도 있다.

Electron에 확장 기능을 로드하려면, Chrome 브라우저에서 다운로드 해야 하며, 파일 시스템 경로를 지정해야 한다. 그리고 BrowserWindow.addDevToolsExtension(extension) API를 호출함으로써 기능을 로드할 수 있다.
예시로 React Developer Tools를 사용한다면.

1. Chrome 브라우저를 설치한다.

2. chrome://extensions로 이동한 후 해시된 fmkadmapgofadopljbjfkapdkoienihi 같이 생긴 확장 기능의 ID를 찾는다.

3. Chrome에서 사용하는 확장 기능을 저장해둔 파일 시스템 경로를 찾는다.

windows에선 %LOCALAPPDATA%\Google\Chrome\User Data\Default\Extensions

Linux에선 ~/.config/google-chrome/Default/Extensions/
~/.config/google-chrome-beta/Default/Extensions/
~/.config/google-chrome-canary/Default/Extensions/
~/.config/chromium/Default/Extensions/

macOS에선 ~/Library/Application Support/Google/Chrome/Default/Extensions

4. Pass the location of the extension to BrowserWindow.addDevToolsExtension API, for the React Developer Tools, it is someting like

```javascript
const path = require('path');
const os = require('os');

BrowserWindow.addDevToolsExtension(
    path.join(os.homedir(), '/Library/Application Support/Google/Chrome/Default/Extensions/fmkadmapgofadopljbjfkapdkoienihi/4.3.0_0')
)
```

참고 : BrowserWindow.addDevToolsExtension API는 app 모듈의 ready 이벤트가 발생하기 전 까지 사용할 수 없다.


개발자 도구를 제거하려면 BrowserWindow.removeDevToolsExtension API를 사용하면 된다.


- 커스텀 드라이버를 이용한 자동화된 테스팅

Electron 응용 프로그램에 대한 자동화 된 테스트를 작성하려면, 응용 프로그램을 "drive"하는 방법이 필요하다. Spectron 은 WebDriver를 통해 사용자 작업을 에뮬레이트 할 수 있는 일반적으로 사용되는 솔류션이다. 그러나 node에 내장된 IPC-over-STDIO를 사용하여 Custom driver를 작성할 수도 있다. custom driver의 이점은 Spectron보다 적은 오버헤드를 요구하는 경향이 있으며, custom method들을 테스트 suite에 표시할 수 있다는 것이다.

```javascript
const childProcess = require('child_process');
const electronPath = require('electron');

// spawn the process
const env = { /* ... */ };
const stdio = ['inherit', 'inherit', 'inherit', 'ipc'];
const appProcess = childProcess.spawn(electronPath, ['./app'], { stdio, env });

// listen for IPC messages from the app
appProcess.on('message', (msg) => {

})

// send an IPC message to the app
appProcess.send({my: 'message'});
```

```javascript
process.on('message', (msg) => {

})

process.send({my: 'message'});
```

