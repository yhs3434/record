- Trouble shooting

    자주 발생하는 트러블들을 정리한 섹션

    - 포트가 이미 사용 중

        metro bundler은 8081 포트를 사용한다. 따라서 다른 프로세스가 8081 포트를 이용중이라면 오류가 발생한다.

- Platform Specific Code

    플랫폼에 따른 코드를 짜고 싶다면 Platform 모듈을 사용하면 된다.

    예시
    ```javascript
    import { Platform, StyleSheet } from 'react-native';

    const styles = StyleSheet.create({
        height: Platform.OS === 'ios' ? 200 : 100
    });
    ```

    Platform.Version으로 버전 탐지 가능

    