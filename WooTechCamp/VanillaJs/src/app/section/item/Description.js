export default class Description {
    $target = null;
    $self = null;

    constructor($target, props) {
        this.$target = $target;
        
        this.$self = document.createElement('p');
        this.$target.appendChild(this.$self);

        this.$self.innerText = props ? props.description : '아이템 설명';
        this.$self.className = 'item-description';
    }
}