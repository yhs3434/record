import React, { useContext } from 'react';
import { ThemeContext } from 'styled-components/native';
import { createStackNavigator } from '@react-navigation/stack';
import { Login, Signup } from '../screens';

const Stack = createStackNavigator();

const AuthStack = () => {
    const theme = useContext(ThemeContext);

    return (
        <Stack.Navigator
            initialRouteName="Login"
            screenOptions={{
                headerTitleAligh: 'center',
                cardStyle: {
                    backgroundColor: theme.backgroundColor,
                }
            }}
        >
            <Stack.Screen name="Login" component={Login} />
            <Stack.Screen name="Signup" component={Signup} />
        </Stack.Navigator>
    );
};

export default AuthStack;