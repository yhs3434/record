코드 분할

1. import 구문 사용
2. React.lazy 사용 (Suspense로 감쌈)

- 예시
```javascript
import React, { Suspense } from 'react';

const OtherComponent = React.lazy(() => import('./OtherComponent'));
const AnotherComponent = React.lazy(() => import('./AnotherComponent'));

function MyComponent() {
  return (
    <div>
      <Suspense fallback={<div>Loading...</div>}>
        <section>
          <OtherComponent />
          <AnotherComponent />
        </section>
      </Suspense>
    </div>
  );
}
```


Context를 이용해서 부모 컴포넌트가 다수의 자식 컴포넌트들에게 props를 전달할 수 있다.
하지만 자주 사용하지는 말고, 가능하면 props를 갖고 있는 컴포넌트 자체를 props로 전달하라.

- 예시
```javascript
function Page(props) {
  const user = props.user;
  const userLink = (
    <Link href={user.permalink}>
      <Avatar user={user} size={props.avatarSize} />
    </Link>
  );
  return <PageLayout userLink={userLink} />;
}

// 이제 이렇게 쓸 수 있습니다.
<Page user={user} avatarSize={avatarSize} />
// ... 그 아래에 ...
<PageLayout userLink={...} />
// ... 그 아래에 ...
<NavigationBar userLink={...} />
// ... 그 아래에 ...
{props.userLink}
```

context를 사용하는 상황이라면 provider를 이용한다.

다수의 context를 사용하여야 한다면,

provider와 consumer를 다수 사용하면 된다.

- 예시
```javascript
<ThemeContext.Provider value={theme}>
    <UserContext.Provider value={signedInUser}>
        <Layout />
    </UserContext.Provider>
</ThemeContext.Provider>

...

<ThemeContext.Consumer>
    {theme => (
    <UserContext.Consumer>
        {user => (
        <ProfilePage user={user} theme={theme} />
        )}
    </UserContext.Consumer>
    )}
</ThemeContext.Consumer>

```