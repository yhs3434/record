const moduleA = {
    state: () => ({hello: 'world'}),
    mutations: {

    },
    actions: {

    },
    getters: {

    }
}

const moduleB = {
    state : () => ({test: 'text'}),
    mutations: {

    },
    actions: {

    }
}

const store = new Vuex.Store({
    modules : {
        a: moduleA,
        b: moduleB
    }
})

store.state.a
store.state.b