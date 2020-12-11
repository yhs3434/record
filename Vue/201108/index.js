var app1 = new Vue({
    el: '#app1',
    data: {
        msg: 'test',
        url: 'https://www.naver.com/'
    },
    methods: {
        doSomething: function (event) {
            console.log('doSomething');
        }
    }
});


var vm = new Vue({
    el: '#example',
    data: {
        message: '안녕하세요'
    },
    computed: {
        // 애로우 함수로 표현하니 this에 해당 vue의 객체가 바인딩 되지 않는 것으로 확인.
        reversedMessage: function () {
            return this.message.split('').reverse().join('');
        }
        
    }
})

var vm2 = new Vue({
    el: '#app3',
    data: {
        firstName: 'Foo',
        lastName: 'Bar',
    },
    watch: {
        fullName: function (val) {
            this.fullName = val + ' ' + this.lastName
        }
    },
    computed: {
        fullName: function() {
            return this.firstName + ' ' + this.lastName + ' computed'
        }
    }
})

var watchExampleVM = new Vue({
    el: '#watch-example',
    data: {
        question: '',
        answer: '질문을 하기 전까지는 대답할 수 없습니다.'
    },
    watch: {
        question: function (newQuestion) {
            this.answer = '입력을 기다리는 중...';
            this.debouncedGetAnswer();
        }
    },
    created: function() {
        this.debouncedGetAnswer = _.debounce(this.debouncedGetAnswer, 500);
    },
    methods: {
        getAnswer: function() {
            if (this.question.indexOf('?') === -1) {
                this.answer = '질문에는 일반적으로 물음표가 포함 됩니다. ;-)'
                return
            }
            this.answer = '생각중...';
            var vm = this;
            axios.get('https://yesno.wtf/api')
            .then(function (response) {
                vm.answer = _.capitalize(response.data.answer);
            })
            .catch(function (error) {
                vm.answer = '에러! API 요청에 오류가 있습니다. ' + error
            })
        }
    }
})