import Text from './footer/Text.js';

export default class _Footer {
    $target = null;
    $self = null;

    constructor($target) {
        this.$target = $target;

        this.$self = document.createElement('footer');
        $target.appendChild(this.$self);

        const $text = new Text(this.$self);
    }
}