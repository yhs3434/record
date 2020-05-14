export default class Text {
    $target = null;
    $self = null;

    constructor($target) {
        this.$target = $target;

        this.$self = document.createElement('h2');
        this.$target.appendChild(this.$self);

        this.$self.innerText = "Footer";
    }
}