export function _createElement(target, tagName) {
    const newElem = document.createElement(tagName);
    target.appendChild(newElem);
    return newElem;
}