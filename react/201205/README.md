키는 형제사이에서만 유일하면 된다.

form 태그 안의 태그의 동작이 기본 html과 리액트가 다름.

주의 :
 select 태그에 multiple 옵션을 허용한다면 value 어트리뷰트에 배열을 전달할 수 있다.
 ```javascript
 <select multiple={true} value={['B','C']}>
 ```

react 공식 사이트에서는 form을 위해 formik을 추천함.

### React로 사고하기

1. UI를 컴포넌트 계층 구조로 나누기
2. React로 정적인 버전 만들기
3. UI state에 대한 최소한의 (하지만 완전한) 표현 찾아내기
4. State가 어디에 있어야 할 지 찾기
5. 역방향 데이터 흐름 추가하기

~ https://ko.reactjs.org/docs/accessibility.html