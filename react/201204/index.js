function Clock (props) {
    return (
        <div>
            <h1>Hello, world!</h1>
            <h2>It is {props.date.toLocaleTimeString()}.</h2>
        </div>
    )
}

function tick () {
    ReactDOM.render(
        <Clock date={new Date()} />,
        document.getElementById('react1')
    );
}

setInterval(tick, 1000);

class Clock2 extends React.Component {
    constructor(props) {
        super(props);
        this.state = {date: new Date()};
    }

    componentDidMount() {
        this.timerID = setInterval(
            () => this.tick(),
            1000
        );
    }

    componentWillUnmount() {
        clearInterval(this.timerID);
    }

    tick() {
        this.setState({
            date: new Date()
        })
    }

    render() {
        return (
            <div>
                <h1>Hello, world!</h1>
                <h2>It is {this.state.date.toLocaleTimeString()}.</h2>
            </div>
        )
    }
}


ReactDOM.render(
    <Clock2 />,
    document.getElementById('react2')
)

const UserGreeting = (props) => {
    return <h1>Welcome back!</h1>;
}

const GuestGreeting = (props) => {
    return <h1>Please sign up.</h1>;
}

const Greeting = (props) => {
    const {isLoggedIn} = props;
    if (isLoggedIn) {
        return <UserGreeting />;
    } else {
        return <GuestGreeting />;
    }
}

ReactDOM.render(
    <Greeting isLoggedIn={false} />,
    document.getElementById('react3')
)