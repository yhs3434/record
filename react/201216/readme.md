- Hook 개요

    hook은 함수 컴포넌트에서 React state와 생명주기 기능을 연동할 수 있게 해주는 함수이다. 즉, class 안에서는 동작하지 않음.

    Hook을 사용하면 구독을 추가하고 제거하는 로직과 같이 서로 관련 있는 코드들을 한군데에 모아서 작성할 수 있다. 반면에 class 컴포넌트에서는 생명주기 메서드 각각에 쪼개서 넣어야만 했다.

    Hook 사용 규칙

    1. 최상위 에서만 Hook을 호출해야 한다. 반복문, 조건문, 중첩된 함수 내에서 Hook을 실행하지 말 것.
    2. React 함수 컴포넌트 내에서만 Hook을 호출해야 한다. 일반 javascript 함수에서는 hook을 호출해서는 안된다.

- State Hook 사용하기

    React의 함수 컴포넌트의 생김새.

    1. 
    ```javascript
    const Example = (props) => {
        return <div />
    }
    ```

    2.
    ```javascript
    function Example(props) {
        return <div />
    }
    ```


    useState 간략 사용 예시.(count)

    ```javascript
    import React, { useState } from 'react';

    function Example() {
        const [count, setCount] = useState(0);

        return (
            <div>
                <p>You clicked {count} times</p>
                <button onClick={() => setCount(count + 1)}>
                    Click me
                </button>
            </div>
        )
    }
    ```

    class 컴포넌트에서는 this.setState가 state를 갱신시키기 위해 병합하였다.

    하지만 functional 컴포넌트에서는 setState는 갱신시키기 위하여 병합이 아니라 대체한다.

- JSX 이해하기

    JSX를 사용하기 위해서는 반드시 React 라이브러리를 스코프 내에 존재시켜야 함.

    예시
    ```javascript
    import React from 'react';
    import CustomButton from './CustomButton';

    function WarningButton() {
    // return React.createElement(CustomButton, {color: 'red'}, null);
    return <CustomButton color="red" />;
    }
    ```

    JSX 내에서도 또한 점 표기법을 사용할 수 있다.

    예시
    ```javascript
    import React from 'react';

    const MyComponents = {
    DatePicker: function DatePicker(props) {
        return <div>Imagine a {props.color} datepicker here.</div>;
    }
    }

    function BlueDatePicker() {
    return <MyComponents.DatePicker color="blue" />;
    }
    ```

    React는 소문자로 시작하는 태그를 HTML 태그로 인식 함. 따라서 컴포넌트를 만든다면 반드시 대문자로 시작하게 만들어야 함.

    prop에 따라 다른 컴포넌트를 render 해야 하는 경우에는 일단 prop에 따라 다른 컴포넌트를 대문자로 시작하는 변수에 담아서 사용함.

    예시
    ```javascript
    import React from 'react';
    import { PhotoStory, VideoStory } from './stories';

    const components = {
    photo: PhotoStory,
    video: VideoStory
    };

    function Story(props) {
    // 올바른 사용법입니다! 대문자로 시작하는 변수는 JSX 타입으로 사용할 수 있습니다.
    const SpecificStory = components[props.storyType];
    return <SpecificStory story={props.story} />;
    }
    ```

    아래 두 컴포넌트는 동일하다

    ```javascript
    function App1() {
        return <Greeting firstName="Ben" lastName="Hector" />;
    }

    function App2() {
        const props = {firstName: 'Ben', lastName: 'Hector'};
        return <Greeting {...props} />;
    }
    ```

    자식으로 콜백 함수를 넘겨도 된다.

    ```javascript
    function Repeat(props) {
        let items = [];
        for (let i = 0; i < props.numTimes; i++) {
            items.push(props.children(i));
        }
        return <div>{items}</div>;
    }

    function ListOfTenThings() {
        return (
            <Repeat numTimes={10}>
            {(index) => <div key={index}>This is item {index} in the list</div>}
            </Repeat>
        );
    }
    ```

- 성능 최적화

    프로덕션 빌드를 활용하라.

    크롬 브라우저의 performance 탭을 사용하여 컴포넌트가 어떻게 마운트, 업데이트, 마운트 해제되는지 시각적으로 볼 수 있다.

    만약 크롬에서 이 작업을 하려면
    1. React 개발 도구를 포함한 모든 Chrome 확장 프로그램을 비활성화한다.
    2. 개발 모드에서 애플리케이션을 실행 중인지 확인한다.
    3. Chrome 개발 도구의 Performance탭을 열고 Record 버튼을 클릭한다.
    4. 프로파일링 할 작업을 진행한다. 20초 이상 기록하지 말 것.
    5. 기록을 멈추라.
    6. React 이벤트는 User Timing 라벨로 그룹화 된다.

    virtual dom을 수정할 때 성능이 눈에 띄게 떨어지는 경우가 있을 수 있다. 만약 굳이 어떤 변화에서는 리렌더링이 필요가 없다면 shouldComponentUpdate() 내에서 false를 반환시키면 된다.

    PureComponent를 상속하면 props와 state를 단순 비교 하므로, shouldComponentUpdate에서 성능적으로 이득을 본 것처럼 만들 수 있다.

    하지만 아래의 경우 정상적인 렌더링이 되지 않는다.

    ```javascript
    class ListOfWords extends React.PureComponent {
    render() {
        return <div>{this.props.words.join(',')}</div>;
    }
    }

    class WordAdder extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
        words: ['marklar']
        };
        this.handleClick = this.handleClick.bind(this);
    }

    handleClick() {
        // This section is bad style and causes a bug
        const words = this.state.words;
        words.push('marklar');
        this.setState({words: words});
    }

    render() {
        return (
        <div>
            <button onClick={this.handleClick} />
            <ListOfWords words={this.state.words} />
        </div>
        );
    }
    }
    ```

    state의 불변성을 지키며 손쉽게 업데이트 될 수 있도록 javascript의 spread syntax를 사용한다.

    객체 또한 변경 없이 손쉽게 업데이트될 수 있도록 object spread properties를 추가하자는 제안이 있다.

- Portal

    ```javascript
    ReactDOM.createPortal(child, container);
    ```
    
    child를 가장 가까운 부모 Node에 렌더링 시키는 것이 아니라, container의 자식으로서 렌더링 시키는 함수이다.

    html DOM 상에서는 하위 컴포넌트가 아니더라도 React DOM 상에서 하위 컴포넌트라면 해당 컴포넌트의 이벤트 버블링은 포착이 된다.