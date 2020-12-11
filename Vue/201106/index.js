var data = {a:1};
var vm = new Vue({
  el: '#example',
  data: data
})

vm.$data === data;
vm.$el === document.getElementById('example');
