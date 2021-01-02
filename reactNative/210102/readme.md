- Handling Text Input

    TextInput은 코어 컴포넌트이고, onChangeText prop과 onSubmitEditing prop을 가진다.

    예시는 단어가 있으면 피자를 보여줌

    더 자세한 기능은 react 공식 문서를 통해 알 수 있음.

- Using a scrollView

    스크린에서 표현할 수 있는 한정적인 수량의 아이템들을 나타낼 때에는 스크롤뷰를 사용하면 된다. 하지만 만약 대량의 아이템을 나타내야 할 때에는 FlatList를 사용하는 것이 좋을 것이다.

- Using List Views

    FlatList는 ScrollView와는 다르게 눈에 보이는 엘리먼트들만 렌더링 한다.

    FlatList는 두개의 props가 필요하다.

    data : 렌더링 될 데이터
    renderItem : 렌더링 될 형식

    SectionList 는 섹션 헤더가 필요할 때 사용하면 됨.

