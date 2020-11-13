new Vue({
    data() {
        return {
            count: 0
        }
    },
    template: `
        <div>{{count}}</div>
    `,
    methods: {
        increment () {
            this.count++;
        }
    }
})