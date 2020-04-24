// 객체 불변성 유지

var copyObject = function (target) {
  var result = {}
  for (var prop in target) {
    result[prop] = target[prop];
  }
  return result;
}

var user = {
  name: 'hansol',
  gender: 'male'
};

var user2 = copyObject(user);

user2.name = 'immortal';

console.log(user === user2);
console.log(user == user2);

console.log(user);
console.log(user2);
