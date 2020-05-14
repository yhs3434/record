import Sample from './Sample.js'
import _Header from './app/_Header.js';
import _Footer from './app/_Footer.js';
import _Nav from './app/_Nav.js';
import _Section from './app/_Section.js';

import {_createElement} from './DomFactory.js';

export default class App {

    $target = null
    dashboard = null

    $header = null;
    $footer = null;
    $section = null;
    $nav = null;
    
    constructor($target) {
        this.$target = $target;

        // this.dashboard = new Sample($target);
        this.$header = new _Header($target);
        this.$nav = new _Nav($target);
        this.$section = new _Section($target);
        this.$footer = new _Footer($target);
    }
}