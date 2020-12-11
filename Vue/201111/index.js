var app1 = new Vue({
    el: '#app1',

    components: {
        'navigation-link': {
            template: `
                <a v-bind:href="url" class="nav-link">
                    <slot></slot>
                </a>
            `,
            props: ['url'],
        }
    }
})

var app2 = new Vue({
    el: '#app2',
    components: {
        'base-layout' : {
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
        }
    }
})

var app3 = new Vue({
    el: '#app3',
    components: {
        'current-user': {
            data: {
                user: {
                    firstName: 'fn',
                    lastName: 'ln'
                }
            },
            template: `
                <span>
                    <slot>{{ user.lastName }}</slot>
                </span>
            `
        }
    }
})

var app5 = new Vue({
    el: '#app5',
    data: {
        data: 'hello',
        foo: 1
    },
    computed: {
        bar: function () {
            console.log('bar');
            return 'barbar';
        }
    },
    methods: {
        baz: function () {
            console.log('baz');
            return 'bazbaz';
        }
    }
})

new Vue({
    el: '#app6',
    data: {
        show: true
    }
})

new Vue({
    el: '#app7',
    data: {
        view: 'v-a'
    },
    components: {
        'v-a' : {
            template: `<div>Component A</div>`
        },
        'v-b': {
            template: `<div>Component B</div>`
        }
    }
})

var mixin = {
    data: function() {
        return {
            message: 'hello',
            foo: 'abc'
        }
    }
}

new Vue({
    el: '#app8',
    mixins: [mixin],
    data: function() {
        return {
            message: 'goodbye',
            bar: 'def'
        }
    },
    created: function() {
        console.log(this.$data);
    }
})

Vue.component('anchored-heading', {
    render: function (createElement) {
        return createElement(
            'h' + this.level,
            this.$slots.default
        )
    },
    props: {
        level: {
            type: Number,
            required: true
        }
    }
})

var getChildrenTextContent = function (children) {
    return children.map(function (node) {
        return node.children
        ? getChildrenTextContent(node.children)
        : node.text
    }).join('')
}

Vue.component('anchored-heading', {
    render: function (createElement) {
        var headingId = getChildrenTextContent(this.$slots.default)
        .toLowerCase();

        return createElement(
            'h' + this.level,
            [
                createElement('a', {
                    attrs: {
                        name: headingId,
                        href: '#' + headingId
                    }
                }, this.$slots.default)
            ]
        )
    },
    props: {
        level: {
            type: Number,
            required: true
        }
    }
})