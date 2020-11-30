const name = 'Josh Perez';
const element = <h1>Hello, {name}</h1>;

ReactDOM.render(
  element,
  document.getElementById('root')
);

function tick() {
    const element = (
        <div>
            <h1>Hello, world!</h1>
            <h2>It is {new Date().toLocaleTimeString()}</h2>
        </div>
    )
    ReactDOM.render(element, document.getElementById('root2'))
}

setInterval(tick, 1000);

const root3 = () => {
    function Welcome(props) {
        return <h1>Hello, {props.name}</h1>
    }

    function App() {
        return (
            <div>
                <Welcome name="Sara" />
                <Welcome name="Cahal" />
                <Welcome name="Edite" />
            </div>
        )
    }

    return App;
}

const App2 = root3();

ReactDOM.render(
    <App2 />,
    document.getElementById('root3')
)