var vm = new Vue({
    el: '.static',
    data: {
        isActive: true,
        hasError: false,
        message: 'test'
    }
})

var vm2 = new Vue({
    el: '#app1',
    data: {
        classObject : {
            active: true,
            'text-danger': false
        },
        activeClass: 'active',
        errorClass: 'text-danger'
    }
})

Vue.component('my-component', {
    template: '<p class="foo bar">Hi</p>'
})

var vm3 = new Vue({
    el: '#app2',
    data: {
        activeColor: 'red',
        fontSize: 30
    }
})

var vm4 = new Vue({
    el: '#app3',
    data: {
        awesome: false
    }
})

var vm5 = new Vue({
    el: '#app4',
    data: {
        loginType: 'username'
    }
})

var example1 = new Vue({
    el: '#example-1',
    data: {
        items: [
            {message: 'Foo'},
            {message: 'Bar'}
        ]
    }
})

var example2 = new Vue({
    el: '#example-2',
    data: {
        parentMessage: 'Parent',
        items: [
            {message: 'Foo'},
            {message: 'Bar'}
        ]
    }
})

var vfor = new Vue({
    el: '#v-for-object',
    data: {
        object: {
            title: 'How to do lists in Vue',
            author: 'Jane Doe',
            publishedAt: '2016-04-10'
        }
    }
})

var up = new Vue({
    el: '#up',
    data: {
        userProfile: {
            name: 'Anika'
        }
    }
})

var vfor2 = new Vue({
    el: '#vfor2',
    data: {
        numbers: [1, 2, 3, 4, 5]
    },
    computed: {
        evenNumbers: function() {
            return this.numbers.filter(function (number) {
                return number % 2 === 0;
            })
        }
    }
})

var vfor3 = new Vue({
    el: '#vfor3',
    data: {
        items: [{msg: 'hello'}, {msg: 'test'}]
    }
})

Vue.component('todo-item', {
    template: `\
        <li>\
            {{ title }}\
            <button v-on:click="$emit(\'remove\')">Remove</button>\
        </li>\
    `,
    props: ['title']
})

new Vue({
    el: '#todo-list-example',
    data: {
        newTodoText: '',
        todos: [
            {
                id: 1,
                title: 'Do the dishes'
            },
            {
                id: 2,
                title: 'Take out the trash'
            },
            {
                id:3,
                title: "Mow the lawn"
            }
        ],
        nextTodoId: 4
    },
    methods: {
        addNewTodo: function () {
            this.todos.push({
                id: this.nextTodoId++,
                title: this.newTodoText
            })
            this.newTodoText = ''
        }
    }
})

var onExample1 = new Vue({
    el: '#on-example-1',
    data: {
        counter : 0
    }
})

var example3 = new Vue({
    el: '#example-3',
    methods: {
        warn: function(message, event) {
            if(event) event.preventDefault();
            console.log(event);
            alert(message);
        }
    }
})

var example4 = new Vue({
    el: '#example-4',
    methods: {
        onPageDown: function() {
            console.log('page down');
        }
    }
})

var ex5 = new Vue({
    el: '#ex-5',
    data: {
        message: ''
    }
})

var ex6 = new Vue({
    el: '#ex-6',
    data: {
        checked: false
    }
})

var ex7 = new Vue({
    el: '#ex-7',
    data: {
        checkedNames: []
    }
})

var ex8 = new Vue({
    el: '#ex-8',
    data: {
        selected: 'A',
        options: [
            {text: 'One', value: 'A'},
            {text: 'Two', value: 'B'},
            {text: 'Three', value: 'C'}
        ]
    }
})