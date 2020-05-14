import SearchItem from './section/SearchItem.js';

export default class _Section {
    $target = null;
    $self = null;

    $items = [];

    constructor($target) {
        this.$target = $target;

        this.$self = document.createElement('section');
        $target.appendChild(this.$self);

        for (let i=0; i<10; i++) {
            this.$items.push(new SearchItem(this.$self, {name: 'hi', description: 'good'}));
        }
    }
}