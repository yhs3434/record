- Profiler

    Profiler의 목적은 메모이제이션 같은 성증 최적화 방법을 활용할 수 있는 애플리케이션의 느린 부분을 식별해내는 것이다.

    profiler의 콜백 props

    ```javascript
    <Component id="Panel" onRender={callback} />

    function onRenderCallback(
        id, // 방금 커밋된 Profiler 트리의 "id"
        phase, // "mount" (트리가 방금 마운트가 된 경우) 혹은 "update"(트리가 리렌더링된 경우)
        actualDuration, // 커밋된 업데이트를 렌더링하는데 걸린 시간
        baseDuration, // 메모이제이션 없이 하위 트리 전체를 렌더링하는데 걸리는 예상시간 
        startTime, // React가 언제 해당 업데이트를 렌더링하기 시작했는지
        commitTime, // React가 해당 업데이트를 언제 커밋했는지
        interactions // 이 업데이트에 해당하는 상호작용들의 집합
    ) {
        // 렌더링 타이밍을 집합하거나 로그...
    }
    ```

- ES6 없이 사용하는 React

    createReactClass()로 클래스형 컴포넌트를 만든다면, 알아서 모든 메소드들을 바인딩 하기 때문에 함수에 this를 바인딩하는 과정이 필요하지 않다.

    새로 작성하는 코드에서는 mixin을 사용하지 말 것.

- JSX 없이 사용하는 React

    JSX 엘리먼트는 사실 React.createElement(component, props, ...children) 를 호출하기 위한 문법 설탕이다. 따라서 JSX로 할 수 있는 모든 것은 순수 JavaScript로도 할 수 있다.

    예를 들어 다음의 JSX로 작성된 코드는

    ```javascript
    class Hello extends React.Component {
        render() {
            return <div>Hello {this.props.toWhat}</div>;
        }
    }

    ReactDOM.render(
        <Hello toWhat="World" />,
        document.getElementById('root')
    );
    ```

    아래처럼 JSX를 사용하지 않은 코드로 컴파일될 수 있다.

    ```javascript
    class Hello extends React.Component {
        render() {
            return React.createElement('div', null, `Hello ${this.props.toWhat}`);
        }
    }

    ReactDOM.render(
        React.createElement(Hello, {toWhat: 'World'}, null),
        document.getElementById('root')
    );
    ```

    만약 React.createElement를 너무 많이 입력하는 것이 피곤하다면 짧은 변수에 할당하는 방법도 있다.

    ```javascript
    const e = React.createElement;

    ReactDOM.render(
        e('div', null 'hello world'),
        document.getElementById('root')
    );
    ```

- 재조정 (Reconciliation)

    두개의 트리를 비교할 때, React는 두 엘리먼트의 루트 엘리먼트부터 비교한다. 이후의 동작은 루트 엘리먼트의 타입에 따라 달라진다.

    1. 엘리먼트의 타입이 다른 경우

        이전 DOM 노드들은 모두 파괴하고 트리 전체를 새로 구죽한다.

        따라서 componentWillUnmount()가 실행되고, 그 이후 componentWillMount()와 componentDidMount()가 이어서 실행 된다.

    2. DOM 엘리먼트의 타입이 같은 경우

        변경된 속성만을 수정한다. 나머지는 그대로 둠.

        생명 주기로 보면, 해당 인스턴스의 componentWillReceiveProps()와 componentWillUpdate()를 호출한다.

    자식에 대한 재귀적인 처리를 할 때, key 속성을 이용한다면 성능이 효율적이게 된다.

    키 값은 데이터의 id 값을 쓰면 유옹할 것.

- effect Hook 사용하기

    리액트가 DOM을 업데이트한 뒤 추가로 코드를 실행해야 하는 경우가 있다. 네트워크 리퀘스트, DOM 수동 조작, 로깅 등은 정리가 필요없는 경우들이다. 이러한 예들은 실행 이후 신경 쓸 것이 없기 때문이다. class와 hook이 이러한 side effects를 어떻게 다르게 구현하는지 비교해보자.

    정리를 이용하는 Effect

    외부 데이터에 구독을 설정해야 하는 경우에는 메모리 누수가 발생하지 않기 위하여 따로 구독을 해제해줘야 한다.

    클래스 컴포넌트에서는 componentWillUnmount()에서 구독을 해제시켜주면 된다.

    예시
    ```javascript
    class FriendStatus extends React.Component {
        constructor(props) {
            super(props);
            this.state = {isOnline: null};
            this.handleStatusChange = this.handleStatusChange.bind(this);
        }

        componentDidMount() {
            ChatAPI.subscribeToFriendStatus(
                this.props.friend.id,
                this.handleStatusChange
            );
        }

        componentWillUnmount() {
            ChatAPI.unsubscribeFromFriendStatus(
                this.props.friend.id,
                this.handleStatusChange
            );
        }

        handleStatusChange(status) {
            this.setState({
                isOnline: status.isOnline
            });
        }

        render() {
            if (this.state.isOnline === null) {
                return 'Loading...';
            }
            return this.state.isOnline ? 'Online' : 'Offline';
        }
    }
    ```

    in Hook
    ```javascript
    function FriendStatus(props) {
        const [isOnline, setIsOnline] = useState(null);

        useEffect(() => {
            function handleStatusChange(status) {
                setIsOnline(status.isOnline);
            }
            ChatAPI.subscribeToFriendStatus(props.friend.id, handleStatusChange);

            return function cleanup() {
                ChatAPI.unsubscribeFromFriendStatus(props.friend.id, handleStatusChange);
            }
        });

        if (isOnline === null) {
            return 'Loading...';
        }
        return isOnline ? 'Online' : 'Offline';
    }
    ```

    클래스형 컴포넌트에서는 componentDidMount()에만 구독을 하고 componentDidUpdate()에서는 구독을 하지 않는다면 버그가 생길 수 있다.

    하지만 함수형 컴포넌트에서는 버그가 생기지 않는다.

    예시
    ```javascript
    function FriendStatus(props) {
        useEffect(() => {
            ChatAPI.subscribeToFriendStatus(props.friend.id, handleStatusChange);
            return () => {
                ChatAPI.unsubscribeFromFriendStatus(prop.friend.id, handleStatusChange);
            }
        })
    }
    ```

- Ref와 DOM

    최대한 ref의 사용을 지양할 것.

    함수형 컴포넌트는 인스턴스가 없기 때문에 createRef를 사용할 수 없다. 만약 함수 컴포넌트에서 ref를 사용할 수 있도록 하려면, forwardRef를 사용하거나 클래스 컴포넌트로 변경할 수 있다.

    - 콜백 ref

    콜백 ref를 사용할 때에는 ref 어트리뷰트에 React.createRef()를 통해 생성된 ref를 전달하는 대신, 함수를 전달한다. 전달된 함수는 다른곳에 저장되고 접근될 수 있는 React 컴포넌트의 인스턴스나 DOM 엘리먼트를 인자로서 받는다.

    아래의 예시는 DOM노드의 참조를 인스턴스의 프로퍼티에 저장하기 위해 ref 콜백을 사용하는 흔한 패턴을 보여준다.

    ```javascript
    class CustomTextInput extends React.Component {
        constructor(props) {
            super(props);

            this.textInput = null;

            this.setTextInputRef = element => {
            this.textInput = element;
            };

            this.focusTextInput = () => {
            // DOM API를 사용하여 text 타입의 input 엘리먼트를 포커스합니다.
            if (this.textInput) this.textInput.focus();
            };
        }

        componentDidMount() {
            // 마운트 되었을 때 자동으로 text 타입의 input 엘리먼트를 포커스합니다.
            this.focusTextInput();
        }

        render() {
            // text 타입의 input 엘리먼트의 참조를 인스턴스의 프로퍼티
            // (예를 들어`this.textInput`)에 저장하기 위해 `ref` 콜백을 사용합니다.
            return (
            <div>
                <input
                type="text"
                ref={this.setTextInputRef}
                />
                <input
                type="button"
                value="Focus the text input"
                onClick={this.focusTextInput}
                />
            </div>
            );
        }
    }
    ```

    