import Sample from './Sample.js'
import Test1 from './Test1.js';
import Test2 from './Test2.js';

export default class App {

    $target = null
    dashboard = null
    dashboard2 = null;
    
    constructor($target) {
        this.$target = $target

        this.buttonInit();

        this.dashboard = new Sample($target)
        this.dashboard2 = new Test1($target);
        this.dashboard3 = new Test2($target);

        this.dashboard2.appendTag();
        this.dashboard2.appendTag();

        this.dashboard3.myAppendChild();
        this.dashboard3.myAppendChild();
    }

    buttonInit() {
        const $button = document.createElement('button');
        $button.innerText = '버튼';
        this.$target.appendChild($button);

        $button.addEventListener('click', () => {
            this.dashboard3.myAppendChild();
        })
    }
}