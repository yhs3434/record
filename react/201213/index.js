const Root1 = (props) => {
    return (
        <div>
            hello
        </div>
    )
}

ReactDOM.render(
    <Root1 />,
    document.getElementById('root1')
);

const Root2Child = React.forwardRef((props, ref) => {
    const buttonClicked = (event) => {
        console.log('clickclick', props);
        console.log(ref);
    }
    return (
        <button ref={ref} onClick={buttonClicked}>
            {props.children}
        </button>
    )
})

class Root2 extends React.Component {
    constructor(props) {
        super(props);
        this.Root2Ref = React.createRef();
        this.buttonClicked = this.buttonClicked.bind(this);
    }

    componentDidUpdate() {
        
    }

    buttonClicked() {
        console.log('click');
    }

    render() {
        return (
            <Root2Child ref={this.Root2Ref} onClick={this.buttonClicked} {...this.props}>
                test22
            </Root2Child>
        )
    }
}

ReactDOM.render(
    <Root2 />,
    document.getElementById('root2')
);