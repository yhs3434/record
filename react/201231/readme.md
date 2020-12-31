- Hook API 참고서

    - 기본 Hook

        useState

        ```javascript
        const [state, setState] = useState(initialState);
        ```

        지연 초기 state

        ```javascript
        const [state, setState] = useState(() => {
            const initialState = someExpensiveComputation(props);
            return initialState;
        })
        ```

    - 추가 Hook

        useReducer

        useState와 기능은 거의 비슷함. 하지만 다음 state가 이전 state에 의존적인 경우 보통 useState보다 useReducer를 선호한다. 또한, useReducer는 자세한 업데이트를 트리거 하는 컴포넌트의 성능을 최적화할 수 있게 한다.

        useCallback

        이는 콜백의 메모이제이션된 버전을 반환한다. 그 메모이제이션된 버전은 콜백의 의존성이 변경되었을 때에만 변경된다. 이것은 불필요한 렌더링을 방지하기 위해 (예로 shouldComponentUpdate를 사용하여) 참조의 동일성에 의존적인 최적화된 자식 컴포넌트에 콜백을 전달할 때 유용하다.

        useCallback(fn, deps)은 useMemo(() => fn, deps)와 같다.

        useMemo

        ```javascript
        const memoizedValue = useMemo(() => computeExpensiveValue(a, b), [a, b]);
        ```

        메모이제이션된 값을 반환한다.

        생성 함수와 그것의 의존성 값의 배열을 전달하세요. useMemo는 의존성이 변경되었을 때에만 메모이제이션된 값만 다시 계산 할 것이다. 이 최적화는 모든 렌더링 시의 고비용 계산을 방지하게 해준다.

        useMemo로 전달된 함수는 렌더링 중에 실행된다는 것을 기억할 것. 통상적으로 렌더링 중에는 하지 않는 것을 이 함수 내에서 하지 말아야 한다. 예를 들어, 사이드 이펙트는 useEffect에서 하는 일이지 useMemo에서 하는 일이 아니다.

        만약 배열이 없는 경우 매 렌더링 때마다 새 값을 계산하게 될 것이다.

        useMemo는 성능 최적화를 위해 사용할 수는 있지만 의미상으로 보장이 있다고 생각하지는 말 것. 가까운 미래의 React에서는 useMemo가 정상적으로 값을 출력하지 않을 수도 있으니, useMemo를 사용하지 않고도 동작할 수 있도록 코드를 작성하고 그것을 추가하여 성능을 최적화하세요.

        useRef

        ```javascript
        const refContainer = useRef(initialValue);
        ```

        useRef는 .current 프로퍼티로 전달된 인자로 초기화된 변경 가능한 ref 객체를 반환한다. 반환된 객체는 컴포넌트의 전 생애주기를 통해 유지될 것이다.

        일반적인 유스케이스는 자식에게 명령적으로 접근하는 경우이다.

        ```javascript
        function TextInputWithFocusButton() {
            const inputEl = useRef(null);
            const onButtonClick = () => {
                inputEl.current.focus();
            };
            return (
                <>
                    <input ref={inputEl} type="text" />
                    <button onClick={ouButtonClick}>Focus the input</button>
                </>
            );
        }
        ```

        본질적으로 useRef는 .current 프로퍼티에 변경 가능한 값을 담고 있는 상자와 같다.

        아마도 우리는 DOM에 접근하는 방법으로 refs에 친숙할 지도 모르겠지만, 만약 <div ref={myRef} />를 사용하여 React로 ref 객체를 전달한다면, React는 모드가 변경될 때마다 변경된 DOM 노드에 그것의 .current 프로퍼티를 설정할 것이다.

        그렇지만, ref 속성보다 useRef()가 더 유용하다. 이 기능은 클래스에서 인스턴스 필드를 사용하는 방법과 유사한 어떤 가변값을 유지하는 데에 편리하다.

        이것은 useRef()가 순수 자바스크립트 객체를 생성하기 때문이다. useRef()와 {current: ...} 객체 자체를 생성하는 것의 유일한 차이점이라면 useRef는 매번 렌더링을 할 때 동일한 ref 객체를 제공한다는 것이다.

        useRef는 내용이 변경될 때 그것을 알려주지는 않는다는 것을 유념하라. .current 프로퍼티를 변형하는 것이 리렌더링을 발생시키지는 않는다. 만약 React가 DOM 노드에 ref를 attach하거나 detach할 때 어떤 코드를 실행하고 싶다면 대신 콜백 ref를 사용하라.


1. 기본 Hook
    1. useState
    2. useEffect
    3. useContext
2. 추가 Hooks
    1. useReducer
    2. useCallback
    3. useMemo
    4. useRef
    5. useImperativeHandle
    6. useLayoutEffect
    7. useDebugValue