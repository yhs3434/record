export default class Test2 {
    $target = null;
    $props = null;
    $state = {
        name: '이건 스테이트 이름입니다',
        text: '이건 스테이트 텍스트입니다'
    };

    constructor($target) {
        this.$target = $target;
    }

    myAppendChild() {
        const child = document.createElement('p');
        child.className = 'test2-child';
        const span1 = document.createElement('span');
        const span2 = document.createElement('span');
        span1.innerText = this.$state.name;
        span2.innerText = this.$state.text;
        
        child.appendChild(span1);
        child.appendChild(span2);
        this.$target.appendChild(child);
        this.$state.name = '변경된 이름';
    }
}