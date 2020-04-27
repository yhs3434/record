var a = 1;
delete global.a;
console.log(a, global.a, this.a);

global.b = 2;
delete b;
console.log(b, global.b, this.b);

// node에서의 동작은 브라우저에서의 동작과 다른 듯.
