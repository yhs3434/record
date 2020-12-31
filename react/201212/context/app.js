import {ThemeContext, themes} from './theme-context';
import ThemedButton from './themed-button';

function Toolbar(props) {
    return (
        <ThemedButton onClick={props.changeTheme}>
            Change Theme
        </ThemedButton>
    )
}

class App extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            theme: themes.light
        };

        this.toggleTheme = () => {
            this.setState(state => ({
                theme:
                state.theme === themes.dark
                    ? themes.light
                    : themes.dark
            }));
        }
    }

    render() {
        <div>
            <ThemeContext.Provider value={this.state.theme}>
                <Toolbar changeTheme={this.toggleTheme} />
            </ThemeContext.Provider>
            <section>
                <ThemedButton />
            </section>
        </div>
    }
}

export default App;