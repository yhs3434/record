export default class _Nav {
    $target = null;
    $self = null;

    constructor($target) {
        this.$target = $target;

        this.$self = document.createElement('nav');
        $target.appendChild(this.$self);
    }
}