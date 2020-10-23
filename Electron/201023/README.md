# 2020년 10월 23일

## 학습 목록

- 배포

일렉트론 앱을 배포하기 위해서는 앱을 재브랜딩하는 과정이 필요하다. 이를 위한 가장 쉬운 방법은 서드파티 패키징 툴을 이용하는 것이다

electron-forge, electron-builder, electron-packager

위의 도구들은 응용 프로그램 패키징, 실행 파일 브랜딩, 올바른 아이콘 설정 및 선택적으로 설치 프로그램 생성과 같은 배포 가능한 Electron 응용 프로그램을 끝내기 위해 수행해야하는 모든 단계를 처리한다.

수동 배포

앱을 배포하기 위해 또한 수동으로 준비해야한다. Electron으로 만든 우리 앱을 배포하기 위해서는 Electron의 prebuilt binaries를 다운로드 해야한다. 먼저, 폴더 이름을 app으로 지정한 후 Electron의 리소스 디렉터리에 폴더를 통째로 집어넣기만 하면 된다. 아래의 예제에서는 electron/에서 가리키는 Electron's prebuilt binaries 위치를 언급해준다.

mac의 경우:

```
electron/Electron.app/Contents/Resources/app/
├── package.json
├── main.js
└── index.html
```

Windows or Linux의 경우:

```
electron/resources/app
├── package.json
├── main.js
└── index.html
```

그리고 Electron.app을 실행하면 Electron앱이 싫행된다. 최종 사용자에겐 이 electron 폴더를 배포하면 된다.

파일로 앱을 패키징 하기

소스파일 전체를 복사해서 배포하는 것과는 별개로 asar 아카이브를 통해 애플리케이션의 소스 코드가 사용자에게 노출되는 것을 방지할 수 있다.

asar아카이브를 사용할 땐 단순히 app 폴더 대신 애플리케이션을 패키징한 app.asar 파일로 대체하면 된다. Electron은 자동으로 app폴더 대신 asar 아카이브를 기반으로 애플리케이션을 실행한다.

macOS의 경우:
```
electron/Electron.app/Contents/Resources/
└── app.asar
```

Windows or Linux의 경우:
```
electron/resources/
└── app.asar
```

리브랜딩 (with Downloaded Binaries)

애플리케이션을 Electron에 번들링한 후 해당 애플리케이션에 맞게 리브랜딩 할 수 있다.

Windows.

rcedit를 통해 electron.exe.을 원하는 이름으로 변경할 수 있고, 또한 아이콘과 기타 정보도 변경할 수 있다.

macOS

Electron.app 을 원하는 이름으로 변경할 수 있다. 그리고 다음 표시된 애플리케이션 내부 파일에서 CFBundleDisplayName, CFBundleIdentifier 그리고 CFBundleName 필드를 원하는 이름으로 변경해야 한다.

또한 helper 앱이 프로세스 모니터에 Electron Helper로 나오지 않도록 이름을 변경할 수 있다. 하지만 반드시 내부 및 모든 helper 앱의 이름을 변경해야 한다.

애플리케이션 이름을 원하는 이름으로 변경한 예시:

```
MyApp.app/Contents
├── Info.plist
├── MacOS/
│   └── MyApp
└── Frameworks/
    └── MyApp Helper.app
        ├── Info.plist
        └── MacOS/
            └── MyApp Helper
```

Linux

실행파일 electron의 이름을 원하는 대로 바꿀 수 있다.

Electron 소스 코드를 다시 빌드하여 리소스 수정하기

제품 이름을 변경하고 소스에서 빌드하여 Electron의 브랜드를 변경할 수도 있다. 이렇게 하려면 args.gn 파일에서 제품 이름에 해당하는 빌드 인수를 설정하고 다시 빌드해야 한다.

- 코드 서명

코드 서명은 이 앱을 내가 만들었다고 인증하는데 사용하는 보안 기술이다.

서명되지 않은 애플리케이션을 배포할 수 있지만 권장되지 않는다.

따라서 작성 중인 Electron 어플리케이션을 패키징 및 배포하고 싶은 경우, 코드 서명하는 것이 좋다.

- Mac App Store

인증서 받기

Apple로부터 인증서를 받는다.

팀 아이디 받기

앱에 서명하기 전에 계정의 팀 ID를 알아야한다. 내 팀 ID는 팀 이름 아래 회원 정보 섹션에 표시된다.

앱 인증

사전 준비를 마친 후, Application Distribution를 따라 애플리케이션을 패키지화하고, 애플리케이션에 서명할 수 있다.

먼저 팀 ID가 값인 ElectronTeamID 키를 앱의 Info.plist에 추가한다.

```
<plist version="1.0">
<dict>
  ...
  <key>ElectronTeamID</key>
  <string>TEAM_ID</string>
</dict>
</plist>
```

- 보안

웹 개발자로서, 우리는 일반적으로 브라우저의 강력한 보안망의 덕을 보고있다. 하지만 Electron은 웹 브라우저를 통한 애플리케이션이 아니다. 따라서 우리는 Electron 앱을 만들기 위해서 보안에 더 신중하게 관심을 기울여야 할 것이다.

최신 Electron 프레임워크로 애플리케이션을 최신 버전으로 유지하기.

의존성 평가하기. NPM이 50만 가지의 재사용 가능한 패키지를 제공하는 반면, 신뢰 가능한 제3자 라이브러리를 고르는 것은 여러분의 책임이다. 알려진 취약점이나 엉망으로 관리되는 코드에 의존하는 오래된 버전의 라이브러리를 사용하면, 애플리케이션 보안이 취약해질 수 있다.

안전한 코딩 사례 채택하기. 애플리케이션 방어의 첫 줄은 여러분의 코드이다. 교차 사이트 스크립팅 같은 흔한 웹 취약점은 Electron 애플리케이션에 높은 보안 영향을 미친다. 그러므로 안전한 소프트웨어 개발 사례를 채택하고 보안 테스트를 수행하기를 매우 권장한다.

Chrome을 사용하는 동안 권한 요청의 경험이 있을 것이다. 웹 사이트에서 사용자가 수동으로 승인해야하는 기능을 사용하려고 시도 할 때마다 팝업이 표시된다.

Why? 기본적으로 Electron은 개발자가 custom handler를 수동으로 구성하지 않는 한, 모든 권한 요청을 자동으로 승인한다. 기본적으로 견고하지만 보안 의식이 있는 개발자라면 견고하지 않다고 가정하고 싶을 것이다.

```javascript
const { session } = require('electron');

session
.fromPartition('some-partition')
.setPermissionRequestHandler((webContents, permission, callback) => {
    const url = webContents.getURL();

    if (permission === 'notifications') {
        callback(true);
    }

    if (!url.startsWith('https://example.com/')) {
        return callback(false)
    }
});
```

WebSecurity를 비활성화 하지 말 것.

```javascript
// 나쁜 예
const mainWindow = new BrowserWindow({
    webPreferences: {
        webSecurity: false
    }
})
```

enableBlinkFeatures를 사용하지 말 것.

experimentalFeatures를 활성화하지 말 것.