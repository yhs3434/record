class MouseTracker extends React.Component {
    constructor (props) {
        super(props);
        this.handleMouseMove = this.handleMouseMove.bind(this);
        this.state = {x: 0, y: 0};
    }

    handleMouseMove(event) {
        this.setState({
            x: event.clientX,
            y: event.clientY
        })
    }

    render() {
        return (
            <div style={{height: '100vh'}} onMouseMove={this.handleMouseMove}>
                <h1>Move the mouse around!</h1>
                <p>The current mouse position is ({this.state.x}, {this.state.y})</p>
            </div>
        )
    }
}

ReactDOM.render(
    <MouseTracker />,
    document.getElementById('root1')
);

class Cat extends React.Component {
    render() {
        const mouse = this.props.mouse;
        return (
            <img src="https://images.mypetlife.co.kr/content/uploads/2019/09/06150205/cat-baby-4208578_1920.jpg"
                style={{ position: 'absolute', width: '100px', height:'auto', left: mouse.x, top: mouse.y }}
            />
        )
    }
}

class Mouse extends React.Component {
    constructor(props) {
        super(props);
        this.handleMouseMove = this.handleMouseMove.bind(this);
        this.state = { x: 0, y: 0 };
    }

    handleMouseMove(event) {
        this.setState({
            x: event.clientX,
            y: event.clientY
        })
    }

    render() {
        return (
            <div style = {{height: '100vh'}} onMouseMove={this.handleMouseMove}>
                {this.props.render(this.state)}
            </div>
        )
    }
}

class MouseTracker2 extends React.Component {
    render() {
        return (
            <div>
                <h1>Move the mouse around!</h1>
                <Mouse render={mouse => (
                    <Cat mouse={mouse} />
                )} />
            </div>
        )
    }
}

ReactDOM.render(
    <MouseTracker2 />,
    document.getElementById('root2')
)

class Greeting extends React.Component {
    render() {
        return (
            <h1>hello, {this.props.name}</h1>
        )
    }
}

Greeting.defaultProps = {
    name: 'Stranger'
};

ReactDOM.render(
    <Greeting />,
    document.getElementById('root3')
);

class FileInput extends React.Component {
    constructor (props) {
        super(props);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.fileInput = React.createRef();
    }

    handleSubmit(event) {
        event.preventDefault();
        alert(
            `Selected file - ${this.fileInput.current.files[0].name}`
        );
    }

    render() {
        return (
            <form onSubmit={this.handleSubmit}>
                <label>
                    Upload file:
                    <input type="file" ref={this.fileInput} />
                </label>
                <br />
                <button type="submit">Submit</button>
            </form>
        )
    }
}

ReactDOM.render(
    <FileInput />,
    document.getElementById('root4')
);