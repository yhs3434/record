## 이벤트 핸들러에 인자 전달하기

```javascript
<button onClick={(e) => this.deleteRow(id, e)}>Delete Row</button>
<button onClick={this.deleteRow.bind(this, id)}>Delete Row</button>
```

~ https://ko.reactjs.org/docs/conditional-rendering.html