export default class HeadName {
    $target = null;
    $self = null;

    constructor($target) {
        this.$target = $target;

        const $h1 = document.createElement('h1');
        $h1.innerText = 'This is Header Component';
        $target.appendChild($h1);
        this.$self = $h1;
    }
}