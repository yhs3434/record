import Name from './item/Name.js';
import Description from './item/Description.js';

export default class SearchItem {
    $target = null;
    $self = null;

    constructor($target, props) {
        this.$target = $target;

        this.$self = document.createElement('article');
        this.$target.appendChild(this.$self);

        const $name = new Name(this.$self, props || null);
        const $description = new Description(this.$self, props || null);

        this.$self.className = 'app-section-searchitem';
    }
}