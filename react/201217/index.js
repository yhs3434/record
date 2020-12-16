const Example = (props) => {
    const [count, setCount] = React.useState(0);

    React.useEffect(() => {
        document.title = `You clicked ${count} times`;
    });

    return (
        <div>
            <p>you clicked {count} times</p>
            <button onClick={() => setCount(count + 1)}>
                Click me
            </button>
        </div>
    )
}

ReactDOM.render(
    <Example />, document.getElementById('root1')
);

function CustomTextInput (props) {
    return (
        <div>
            <input ref={props.inputRef} />
        </div>
    )
}

class Parent extends React.Component {
    componentDidMount() {
        console.log('Parent', this.inputElement);
    }

    render() {
        return (
            <CustomTextInput
                inputRef={el => this.inputElement = el}
            />
        )
    }
}

ReactDOM.render(
    <Parent />,
    document.getElementById('root2')
);