export default class Name {
    $target = null;
    $self = null;

    constructor($target, props) {
        this.$target = $target;
        
        this.$self = document.createElement('p');
        this.$target.appendChild(this.$self);

        this.$self.innerText = props ? props.name : '아이템 이름';
        this.$self.className = 'item-name';
    }
}