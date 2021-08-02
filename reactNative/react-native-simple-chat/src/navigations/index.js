import React from 'react';
import { NavigationContainer } from '@react-navigation/native';
import AuthStack from './AuthStack';
import { Spinner } from '../components';

const Navigation = () => {
    return (
        <NavigationContainer>
            <AuthStack />
            <Spinner />
        </NavigationContainer>
    )
};

export default Navigation;