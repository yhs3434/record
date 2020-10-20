# 2020년 10월 19일

## 다짐

모든 자존심을 버려라. 자기 자신을 객관적으로 냉정하게 판단해라. 난 지금 을이다. 높이 올라가기 위해서는 항상 위를 바라보며 살아야한다. 위만을 바라보며 산다면, 난 평생 을로 살아야할 수도 있다. 그렇게 살아라. 평생을 을로 살 각오를 해라. 그래야 성공할 수 있을 것이다.

## 학습한 목록

- 알림

```javascript
const myNotification = new Notification('Title', {
    body: 'Lorem Ipsum Dolor Sit Amet'
});

myNotification.onclick = () => {
    console.log('Notification clicked');
};
```

알림용 객체를 Notification 클래스의 생성자를 통해 생성한다.
그 이후 자바스크립트의 이벤트 드리븐 프로그래밍 방식으로 알림 객체에 이벤트를 심어 사용하면 될 것이다.

윈도우에서는 각 버전마다 알림의 권한을 얻기위한 방법이 따로 존재한다.
맥이나 리눅스는 간단한 것 같음. (써보진 않아서 모름)


- 최근 문서

```javascript
const { app } = require('electron');
app.addRecentDocument(process.execPath);
```

를 통해 최근 문서 리스트를 등록할 수 있으며

```javascript
const { app } = require('electron');
app.clearRecentDocuments();
```

를 통해 최근 문서 리스트를 비울 수 있다.


- progress bar

```javascript
const { BrowserWindow } = require('electron');
const win = new BrowserWindow();

win.setProgressBar(0.5);
```

win 객체(브라우저 윈도우)에 setProgressBar 메소드를 통해 구현할 수 있다.
간단함.

- mac OS Custom Dock

커스텀 독은 일반적으로 어플의 기능을 모두 이용하지 않고 어느 기능만 바로가기로 사용할 수 있도록 사용하는 것이라 한다.

맥을 한 번도 써본 적이 없어서 잘 모르겠지만 아마 안드로이드의 바로가기 기능과 비슷한 것으로 예상한다.

아무튼 사용법은

```javascript
const { app } = require('electron');

const dockMenu = Menu.buildFromTemplate([
    {
        label: 'New Window',
        click () {console.log('New Window') }
    }, {
        label: 'New Window with Settings',
        submenu: [
            { label: 'Basic' },
            { label: 'Pro' }
        ]
    },
    { label: 'New Command...' }
]);

app.dock.setMenu(dockMenu);
```

- 윈도우 작업 표시줄 커스텀

점프리스트 생성하는 법

```javascript
const { app } = require('electron');

app.setUserTasks([
    {
        program: process.execPath,
        arguments: '--new-window',
        iconPath: process,execPath,
        iconIndex: 0,
        title: 'New Window',
        description: 'Create a new window'
    }
]);
```

만약 작업 목록을 정리하고 싶다면 빈 배열로 setUserTasks 메소드를 호출하면 됨.

```javascript
const { app } = require('electron');
app.setUserTasks([]);
```

미리보기 툴바를 추가할 수도 있다.
참고로 미리보기 툴바는 동영상같은거 실행중일 때 작업표시줄에 마우스 갖다대면, 미리보기로 영상 보여주는 기능이다.

```javascript
const { BrowserWindow } = require('electron');
const path = require('path');

const win = new BrowserWindow();

win.setThumbarButtons([
    {
        tooltip: 'button1',
        icon: path.join(__dirname, 'button1.png'),
        click () {console.log('button1 clicked')}
    }, {
        tooltip: 'button2',
        icon: path.join(__dirname, 'button2.png'),
        flags: ['enabled', 'dismissonClick'],
        click () { console.log('button2 clicked.') }
    }
]);
```

만약 미리보기 툴바를 비우려면 점프리스트 때와 마찬가지로 빈 배열을 전달하면 됨.

```javascript
const { BrowserWindow } = require('electron');

const win = new BrowserWindow();
win.setThumbarButtons([]);
```

오버레이 아이콘을 설정하는 방법.

```javascript
const { BrowserWindow } = require('electron');
const win = new BrowserWindow();
win.setOverlayIcon('path/to/overlay.png', 'Description for overlay');
```

깜빡이는 창 설정하는 방법.

```javascript
const { BrowserWindow } = require('electron');
const win = new BrowserWindow();
win.once('focus', () => win.flashFrame(false));
win.flashFrame(true);
```

- 커스텀 리눅스 데스크톱 동작

리눅스에서 해야할 듯. 노드 자체에서는 커스텀 기능이 없는 듯 함. 리눅스를 커스텀해서 사용하는 편이 아니라 잘 모르겠음.

- 키보드 단축키

  + 로컬 단축키

우선 Menu 모듈을 사용해야 한다. 이후 MenuItem이 만들어질 때 [accelerator] 프로퍼티를 명기한다.

```javascript
const { Menu, MenuItem } = require('electron');
const menu = new Menu();

menu.append(new MenuItem({
    label: 'Print',
    accelerator: 'CmdOrCtrl+P',
    click: () => { console.log('time to print stuff') }
}));
```

만약 유저의 운영체제에 따라 단축키를 다르게 등록하고 싶다면

```javascript
{
    accelerator: process.platform === 'darwin' ? 'Alt+Cmd+I' : 'Ctrl+Shift+I'
}
```

즉, 삼항연산자를 통해 다르게 표현. 리액트에서 많이 사용해서 익숙함.


  + 전역 단축키

애플리케이션이 키보드 포커스를 갖지 않을 때에도 키보드 이벤트를 감지하는 globalShortcut 모듈을 사용할 수 있다.

```javascript
const { app, globalShortcut } = require('electron');

app.whenReady().then(() => {
    globalShortcut.register('CommandOrControl+X', () => {
        console.log('CommandOrControl+X is pressed');
    })
});
```

  + BrowserWindow 간의 단축키

  만약 BrowserWindow에 사용될 키보드 단축키를 다루길 원한다면, 렌더러 프로세스 안에 있는 윈도우 오브젝트 상에 keyup과 keydown 이벤트 리스너를 사용해야 한다.

  ```javascript
  window.addEventListener('keyup', doSomething, true);
  ```
  
  세번째 파라미너 true는 다른 리스너가 키 입력을 수신하기 전에 해당 리스너가 항상 수신하겠다는 의미이다. 그러므로 다른 리스너는 stopPropagation()을 호출할 수 없다.

  before-input-event 이벤트는 페이지 안에서 keydown과 keyup 이벤트를 전달하기 전에 실행된다. 이는 메뉴 안에서 보이지 않는 사용자 정의 단축키를 캐치하여 처리할 것이다.

  만약 수동으로 단축키 파싱하는 것을 원하지 않는다면 mousetrap과 같은 진보된 키 감지 라이브러리들이 존재한다.

  ```javascript
  Mousetrap.bind('4', () => { console.log('4'); });
  MouseTrap.bind('?', () => { console.log('show shortcut!') });
  Mousetrap.bind('esc', () => { console.log('escape') }, 'keyup');

  // 조합들
  Mousetrap.bind('command+shift+k', () => { console.log('command shift k') });

  // 같은 콜백에 다수 조합을 매핑
  Mousetrap.bind(['command+k', 'ctrl+k'], () => {
      console.log('command k or control k');

      // 기본 동작을 막고 버블링으로부터 이벤트를 멈추기 위해 false를 반환
      return false
  });

  // 지메일 스타일 시퀀스
  Mousetrap.bind('g i', () => { console.log('go to inbox') });
  Mousetrap.bind('* a', () => { console.log('select all') });

  // 코나미 코드! ㅋㅋ 철권할 떄 생각나네
  Mousetrap.bind('up up down down left right left right b a enter', () => {
      console.log('konami code');
  });
```

- 오프라인/온라인 감지

온라인 및 오프라인 이벤트 감지는 표준 HTML5 API의 일부인 navigator.onLine 특성을 사용하여 렌더러 프로세스에서 구현할 수 있다.

navigator.onLine 속성은 네트워크 요청이 실패가 확실할 떄 false를 반환하며, 그 나머지의 경우에는 모두 true를 리턴한다.

네트워크 끊김 확실 : false
이외의 상황 : true

예시:

```javascript
const { app, BrowserWindow } = require('electron');

let onlineStatusWindow;

app.whenReady().then(() => {
    onlineStatusWindow = new BrowserWindow({ width: 0, height: 0, show: false });
    onlineStatusWindow.loadURL(`file://${__dirname}/online-status.html`);
});
```

```html
<!DOCTYPE html>
<html>
    <body>
        <script>
            const alertOnlineStatus = () => {
                window.alert(navigator.onLine ? 'online' : 'offline');
            }

            window.addEventListener('online', alertOnlineStatus);
            window.addEventListener('offline', alertOnlineStatus);

            alertOnlineStatus();
        </script>
    </body>
</html>
```

주 프로세스에서 이러한 이벤트에 응답하려는 경우도 있을 수 있다. 그러나 메인 프로세스는 navigator 객체가 없으므로 이러한 이벤트를 직접 감지할 수 없다. Electron의 프로세스 간 통신 유틸리티를 사용하면 이벤트를 전달할 수 있다. 다음 예제와 같이 주 프로세스로 이동하여 필요에 따라 처리한다.

```javascript
const { app, BrowserWindow, ipcMain } = require('electron');
let onlineStatusWindow;

app.whenReady().then(() => {
    onlineStatusWindow = new BrowserWindow({ width: 0, height: 0, show: false, webPreferences: { nodeIntegration: true } })
    onlineStatusWindow.loadURL(`file://${__dirname}/online-status.html`);
});

ipcMain.on('online-status-changed', (event, status) => {
    console.log(status);
});
```

```html
<!DOCTYPE html>
<html>
<body>
<script>
    const { ipcRendered } = require('electron');
    const updateOnlineStatus = () => {
        ipcRendered.send('online-status-changed', navigator.onLine ? 'online', 'offline' )
    }

    window.addEventListener('online', updateOnlineStatus);
    window.addEventListener('offline', updateOnlineStatus);

    updateOnlineStatus();
</script>
</body>
</html>
```

