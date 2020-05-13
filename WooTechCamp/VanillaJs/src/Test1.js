export default class Test1 {

    $target = null;
    $child = null;

    constructor ($target) {
        this.$target = $target;
    }

    appendTag () {
        this.$child = document.createElement('p');
        this.$child.innerHTML = `<span>this is test text~</span>`;

        this.$target.appendChild(this.$child);
    }
}