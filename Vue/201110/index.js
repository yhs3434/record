
var app1 = new Vue({
    el: '#app1',
    data: {
        picked: '',
        toggle: [],
        selected: []
    }
})

var app2 = new Vue({
    el: '#app2',
    data: {
        toggle: ''
    }
})

var app3 = new Vue({
    el: '#app3',
    data: {
        selected: ''
    }
})

Vue.component('my-component', {
    template: '<div>사용자 정의 컴포넌트 입니다!</div>'
})

/*
new Vue({
    el: '#app4',
})
*/

var Child = {
    template: '<div>사용자 정의 컴포넌트 입니다!</div>'
}

new Vue({
    el: '#app4',
    components: {
        'my-component': Child
    }
})

var data = {counter: 0};

new Vue({
    el: '#app5',
    components: {
        'simple-counter': {
            template: '<button v-on:click="counter += 1">{{counter}}</button>',
            data: function() {
                return data;
            }
        }
    }
})

new Vue({
    el: '#app6',
    components: {
        'simple-counter': {
            template: '<button v-on:click="counter += 1">{{counter}}</button>',
            data: function() {
                return {
                    counter : 0
                }
            }
        }
    }
})

new Vue({
    el: '#app7',
    components: {
        'child': {
            props: ['message'],
            template: '<span>{{message}}</span>'
        }
    }
})

new Vue({
    el: '#app8',
    data: {
        parentMsg: ''
    },
    components: {
        child: {
            props: ['myMessage'],
            template: '<span>{{myMessage}}</span>'
        }
    }
})

Vue.component('button-counter', {
    template: '<button v-on:click="incrementCounter">{{counter}}</button>',
    data: function() {
        return {
            counter: 0
        }
    },
    methods: {
        incrementCounter: function () {
            this.counter += 1;
            this.$emit('increment');
        }
    }
})

new Vue({
    el: '#app9',
    data: {
        total: 0
    },
    methods: {
        incrementTotal: function() {
            this.total += 1;
        }
    }
})

Vue.component('currency-input', {
    template: `
    <span>
        $
        <input ref="input" v-bind:value="value" v-on:input="updateValue($event.target.value)">
    </span>`,
    props: ['value'],
    methods: {
        updateValue: function (value) {
            var formattedValue = value.trim().slice(0, value.indexOf('.') === -1 ? value.length : value.indexOf('.') + 3)
            if (formattedValue !== value) {
                this.$refs.input.value = formattedValue;
            }
            this.$emit('input', Number(formattedValue))
        }
    }
})

new Vue({
    el: '#app10',
    data: {
        price: 0
    }
})

Vue.component('my-checkbox', {
    model: {
        prop: 'checked',
        event: 'change'
    },
    props: {
        checked: Boolean,
        value: String
    }
})

Vue.component('my-component', {
    template: `
        <div>
            <h2>나는 자식 컴포넌트의 제목입니다.</h2>
            <slot>
                제공된 컨텐츠가 없는 경우에만 보실 수 있습니다.
            </slot>
        </div>
    `
})

new Vue({
    el: '#app11'
})

Vue.component('app-layout', {
    template: `
        <div class="container">
            <header>
                <slot name="header"></slot>
            </header>
            <main>
                <slot></slot>
            </main>
            <footer>
                <slot name="footer"></slot>
            </footer>
        </div>
    `
})

new Vue({
    el: '#app12'
})

var app14 = new Vue({
    el: '#app14',
    data: {
        currentView: 'home'
    },
    components: {
        home: {
            template: `<span>home</span>`
        },
        posts: {
            template: `<span>posts</span>`
        },
        archieve: {
            template: `<span>archieve</span>`
        }
    }
})

var app15 = new Vue({el: '#app15'});

Vue.component('async-example', function (resolve, reject) {
    setTimeout(function () {
        resolve({
            template: '<div>I am async!</div>'
        })
    }, 1000);
})

var app16 = new Vue({
    el: '#app16',
    data: {
        username: ''
    },
    components: {
        'base-input': {
            inheritAttrs: false,
            props: ['label', 'value'],
            template: `
                <label>
                    {{ label }}
                    <input
                        v-bind="$attrs"
                        v-bind:value="value"
                        v-on:input="$emit('input', $event.target.value)"
                    >
                </label>
            `
        }
    }
})

var app17 = new Vue({
    el: '#app17',
    data: {
        username: ''
    },
    components: {
        'base-checkbox': {
            model: {
                prop: 'checked',
                event: 'change'
            },
            props: {
                checked: Boolean
            },
            template: `
                <input
                    type="checkbox"
                    v-bind:checked="checked"
                    v-on:change="$emit('change', $event.target.checked)"
                >
            `
        }
    }
})

Vue.component('base-input', {
    inheritAttrs: false,
    props: ['label', 'value'],
    computed: {
        inputListener: function () {
            var vm = this;
            return Object.assign({},
                this.$listeners,
                {
                    input: function (event) {
                        vm.$emit('input', event.target.value)
                    }
                })
        }
    },
    template:`
        <label>
            {{ label }}
            <input
                v-bind="$attrs"
                v-bind:value="value"
                v-on="inputListener"
            >
        </label>
    `
})

new Vue({el: '#app17'})