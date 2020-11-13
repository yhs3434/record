const store = new Vuex.Store({
    state: {
        count: 0
    },
    mutations: {
        increment: state => state.count++,
        decrement: state => state.count--
    }
})

var app1 = new Vue({
    el: '#app1',
    computed: {
        count (){
            return store.state.count
        }
    },
    methods: {
        increment () {
            store.commit('increment');
        },
        decrement () {
            store.commit('decrement');
        }
    }
})

const Counter = {
    template : `<div>{{count}}</div>`,
    computed: {
        count() {
            return store.state.count;
        }
    }
}

const app = new Vue({
    
})