var obj = {
  outer : function () {
    console.log(this);
    var innerFunc = function () {
      console.log(this);
    }.bind(this);
    innerFunc();
  }
}
obj.outer();
