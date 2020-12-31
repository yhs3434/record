- 에러 경계
    
    에러가 발생한 뒤에 폴백 UI를 렌더링하려면 static getDerivedStateFromError()를 사용.

    에러 정보를 기록하려면 componentDidCatch()를 사용.

    예시
    ```javascript
    class ErrorBoundary extends React.Component {
        constructor(props) {
            super(props);
            this.state = { hasError: false };
        }

        static getDerivedStateFromError(error) {
            return { hasError: true };
        }

        componentDidCatch(error, errorInfo) {
            logErrorToMyService(error, errorInfo);
        }

        render() {
            if (this.state.hasError) {
                return <h1>Someting went wrong.</h1>;
            }

            return this.props.children;
        }
    }

    그 후

    <ErrorBoundary>
        <MyWidget />
    </ErrorBoundary>
    ```

    만약 이벤트 핸들러 내부의 에러를 잡아야 하는 경우에는 일반 자바스크립트의 try / catch 구문을 사용한다.

    예시
    ```javascript
    class MyComponent extends React.Component {
        constructor(props) {
            super(props);
            this.state = { error: null };
            this.handleClick = this.handleClick.bind(this);
        }

        handleClick() {
            try {
                // 에러를 던질 수 있는 무언가
            } catch (error) {
                this.setState({error});
            }
        }

        render() {
            if (this.state.error) {
                return <h1>Caught an error.</h1>
            }
            return <button onClick={this.handleClick}>Click</button>
        }
    }
    ```

- Ref 전달하기

    ```javascript
    function FancyButton(props) {
        return (
            <button className="FancyButton">
                {props.children}
            </button>
        )
    }
    ```
    
    만약 상위 컴포넌트에서 정의한 ref를 사용하여야 하는 경우가 있을 것이다.

    그럴 때는 아래의 예시처럼 leaf 컴포넌트를 만들면 된다.

    ```javascript
    const FancyButton = React.forwardRef((props, ref) => (
        <button ref={ref} className="FancyButton">
            {props.children}
        </button>
    ));

    const ref = React.createRef();
    <FancyButton ref={ref}>Click me!</FancyButton>;
    ```

    HOC의 내부에 존재하는 컴포넌트에서 ref를 사용하고 싶다면, forwardRef를 통해서 내부 컴포넌트에 ref를 전달해주어야 한다.

    예시

    ```javascript
    function logProps(Component) {
        class LogProps extends React.Component {
            componentDidUpdate(prevProps) {
                console.log('old props:', prevProps);
                console.log('new props:', this.props);
            }

            render() {
                const {forwardedRef, ...rest} = this.props;

                return <Component ref={forwardedRef} {...rest} />;
            }
        }

        return React.forwardRef((props, ref) => {
            return <LogProps {...props} forwardedRef={ref} />;
        })
    }


    const WrappedComponent = React.forwardRef((props, ref) => {
        return <LogProps {...props} forwardedRef={ref} />;
    });
    ```

- Fragment

    React에서 컴포넌트가 여러 엘리먼트를 반환하는 것은 흔한 패턴이다. Fragments는 DOM에 별도의 노드를 추가하지 않고 여러 자식을 그룹화할 수 있다.

    예시
    ```javascript
    render() {
        return (
            <React.Fragment>
                <ChildA />
                <ChildB />
                <ChildC />
            </React.Fragment>
        )
    }
    ```

- 1. Hook 소개

    Facebook은 많은 컴포넌트를 만들어본 결과 클래스의 유연성에 문제가 있다고 생각했고, 훅을 만들기로 결정하였다.

    하지만 이미 만들어놓은 수만개의 클래스형 컴포넌트가 있기 때문에, 클래스형 리액트를 없앨 생각은 없다.