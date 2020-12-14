- 고차 컴포넌트

    ```javascript
    function logProps(InputComponent) {
        InputComponent.prototype.componentDidUpdate = function(prevProps) {
            console.log('Current props: ', this.props);
            console.log('Previous props: ', prevProps);
        };

        return InputComponent;
    }

    const EnhancedComponent = logProps(InputComponent);
    ```

    위의 코드의 문제는 만약 함수형 컴포넌트처럼 프로토타입이 componentDidUpdate를 가지지 않는다면, 정상적으로 동작하지 않는다는 것이다.

    따라서 밑에처럼 HOC로 만든다.

    ```javascript
    function logProps(WrappedComponent) {
        return class extends React.Component {
            componentDidUpdate(prevProps) {
                console.log('Current props: ', this.props);
                console.log('Previous props: ', prevProps);
            }

            render() {
                return <WrappedComponent {...this.props} />
            }
        }
    }
    ```

    관계 없는 props들 통과시키기.

    예시
    ```javascript
    render() {
        const { extraProp, ...passThroughProps } = this.props;

        const injectedProp = someStateOrInstanceMethod;

        return (
            <WrappedComponent
                injectedProp={injectedProp}
                {...passThroughProps}
            >
        )
    }
    ```

    HOC 몇 가지 주의사항
    1. render method 안에서 HOC를 사용하지 말 것
    2. 정적 메소드는 복사해서 사용할 것. (웬만해서는 정적 메소드 쓰지 마셈)
    3. ref는 통과되지 않음.

- 다른 라이브러리와 통합하기

    어떤 컴포넌트에서 jquery를 사용하기.

    ```javascript
    class Chosen extends React.Component {
        componentDidMount() {
            this.$el = $(this.el);
            this.$el.chosen();

            this.handleChange = this.handleChange.bind(this);
            this.$el.on('change', this.handleChange);
        }

        componentDidUpdate(prevProps) {
            if(prevProps.children !== this.props.children) {
                this.$el.trigger("chosen:updated");
            }
        }

        componentWillUnmount() {
            this.$el.off('change', this.handleChange);
            this.$el.chosen('destroy');
        }

        handleChange(e) {
            this.props.onChange(e.target.value);
        }

        render() {
            return (
                <div>
                    <select className="Chosen-select" ref={el => this.el = el}>
                        {this.props.children}
                    </select>
                </div>
            )
        }
    }
    ```

    ReactDOM.render()를 여러 곳에서 여러번 사용한다면 react를 msa로 만드는 것이 가능하다.

    예시
    ```javascript
    function Button(props) {
        return <button onClick={props.onClick}>Say Hello</button>;
    }

    function HelloButton() {
        function handleClick() {
            alert('Hello!');
        }
        return <Button onClick={handleClick} />
    }

    ReactDOM.render(
        <HelloButton />,
        document.getElementById('container')
    )
    ```

    react는 props를 이용하는 방식으로 다른 라이브러리들을 유연하게 사용할 수 있다.