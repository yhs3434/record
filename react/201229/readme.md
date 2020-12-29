- 자신만의 Hook 만들기

    사용자 정의 Hook은 이름이 use로 시작하는 자바스크립트 함수이다. 사용자 Hook은 다른 Hook을 호출할 수 있다. 예를 들자면, 아래의 useFriendStatus가 우리의 첫 번째 사용자 정의 Hook이다.

    예시
    ```javascript
    import { useState, useEffect } from 'react';

    function useFriendStatus(friendID) {
        const [isOnline, setIsOnline] = useState(null);

        useEffect(() => {
            function handleStatusChange(status) {
                setIsOnline(status.isOnline);
            }

            ChatAPI.subscribeToFriendStatus(friendID, handleStatusChange);
            return () => {
                ChatAPI.unsubscribeFromFriendStatus(friendID, handleStatusChange);
            }
        })

        return isOnline;
    }
    ```

    이름은 반드시 use로 시작해야 하는데, 그래야만 한눈에 보아도 Hook 규칙이 적용되는지를 파악할 수 있기 때문이다.

    사용자 정의 Hook은 React의 특별한 기능이라기보다 기본적으로 Hook의 디자인을 따르는 관습이다.

    같은 Hook을 사용하는 두 개의 컴포넌트는 state를 공유하지 않는다. 