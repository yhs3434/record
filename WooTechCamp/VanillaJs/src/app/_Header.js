import HeadName from './header/HeadName.js';

export default class _Header {
    $target = null;
    $self = null;

    constructor($target) {
        this.$target = $target;

        this.$self = document.createElement('header');
        $target.appendChild(this.$self);

        const $headName = new HeadName(this.$self);
    }
}