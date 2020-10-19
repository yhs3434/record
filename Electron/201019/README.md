# 2020년 10월 19일

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
```

app.dock.setMenu(dockMenu);

- 윈도우 작업 표시줄 커스텀

