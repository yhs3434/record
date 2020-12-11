setTimeout(function () {console.log(this);}, 300);
setTimeout(() => {console.log(this);}, 300);

[1, 2, 3, 4, 5].forEach(function (x) {
  console.log(this, x);
})

[1, 2, 3, 4, 5].forEach((x) => {
  console.log(this, x);
})

var pelem = document.createElement('p');
pelem.innerText = 'click!';
pelem.className = 'click';
document.body.appendChild(pelem);

document.body.querySelector('.click').addEventListener('click', function (e) {
  console.log(this, e);
})
// this === pelem

document.body.querySelector('.click').addEventListener('click', (e) => {
  console.log(this, e);
})
// this === window (|| global)
