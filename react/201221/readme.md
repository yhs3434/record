- Render Props

    render prop을 사용하면 HOC도 쉽게 만들 수 있음.

    ```javascript
    function withMouse(Component) {
        return class extends React.Component {
            render() {
                return (
                    <Mouse render={ mouse=> (
                        <Component {...this.props} mouse={mouse} />
                    )}>
                )
            }
        }
    }
    ```

- 정적 타입 검사

    1. Flow
    2. typescript
    3. Reason
    4. Kotlin
    5. etc...

- Strict Mode

    * strict 모드는 개발 모드에서만 활성화되므로 프로덕션 빌드에는 영향을 끼치지 않는다.

    Strict 모드는 모든 문자열 ref의 사용에 대하여 경고한다.

    findDOMNode를 사용하기보다 ref를 사용하여 부모까지 끌어올리는 형태로 사용할 것.

    strict 모드는 의도적으로 메소드를 두 번 호출하기 때문에, 일관성이 없어 발견하기 힘든 버그를 찾을 수 있도록 한다. (100%는 아님)

- PropTypes를 사용한 타입 검사

    앱이 커질수록 타입 확인을 통해 많은 버그를 잡을 수 있다. 특정 앱에서는 Flow 또는 TypeScript 같은 Javascript 도구를 사용할수 있다. 하지만 당신이 이러한 도구를 사용하지 않더라도 React는 내장된 타입 확인 기능들을 가지고 있다. 컴포넌트의 props에 타입 확인을 하려면 다음과 같이 특별한 프로퍼티인 propTypes를 선언할 수 있다.

    ```javascript
    import PropTypes from 'prop-types';

    class Greeting extends React.Component {
        render() {
            return (
                <h1>Hello, {this.props.name}</h1>
            )
        }
    }

    Greeting.propTypes = {
        name: PropTypes.string
    };
    ```

    propTypes는 성능상의 이유로 개발 모드에서만 확인될 것입니다.

    아래는 제공된 서로 다른 유효성 검사기들을 보여주는 예시이다.

    ```javascript
    import PropTypes from 'prop-types';

    MyComponent.propTypes = {
        optionalArray: PropTypes.array,
        optionalBool: PropTypes.bool,
        optionalFunc: PropTypes.func,
        optionalNumber: PropTypes.number,
        optionalObject: PropTypes.object,
        optionalString: PropTypes.string,
        optionalSymbol: PropTypes.symbol,
        optionalNode: PropTypes.node,
        optionalElement: PropTypes.element,
        optionalElementType: PropTypes.elementType,
        optionalMessage: PropTypes.instanceOf(Message),
        optionalEnum: PropTypes.oneOf(['News', 'Photos']),
        optionalUnion: PropTypes.oneOfType([
            PropTypes.string,
            PropTypes.number,
            PropTypes,instanceOf(Message)
        ])

        ...

    }
    ```

    defaultProps를 사용하는 초기값을 정의할 수 있다.

    함수형 컴포넌트에서는 export default를 나중에 선언하면 됨.

- 비제어 컴포넌트

    대부분의 경우에는 폼을 구현하는데 있어 제어 컴포넌트를 사용하는 것이 좋다.

    하지만 ref를 통해 직접 DOM을 조작해야 하는 경우가 있다.

    예시) 파일

- 웹 컴포넌트

    웹 컴포넌트는 className이 아닌 class를 사용한다.

    웹 컴포넌트에서 React 사용하기

    ```javascript
    class XSearch extends HTMLElement {
        connectedCallback() {
            const mountPoint = document.createElement('span');
            this.attachShadow({ mode: 'open' }).appendChild(mountPoint);

            const name = this.getAttribute('name');
            const url = 'https://www.google.com/search?q=' + encodeURIComponent(name);
            ReactDOM.render(<a href={url}>{name}</a>, mountPoint);
        }
    }
    customElements.define('x-search', XSearch);
    ```

    