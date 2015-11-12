/*
 A very lightweight implementation of CommonJS Asynchronous Module Definition and
 a compatible loader.
*/
var require, define;
(function () {
  var modules = {};
  define = function (name, deps, factory) {
    modules[name] = { deps: deps, factory: factory };
  };
  function load(name) {
    var i, args;
    if (typeof modules[name] == 'undefined') {
      throw new Error('Cannot require undefined module: ' + name);
    }
    if (typeof modules[name].value == 'undefined') {
      modules[name].value = require(modules[name].deps, modules[name].factory);
    }
    return modules[name].value;
  }
  require = function(deps, callback) {
    var i, args = new Array(deps.length);
    for (i = 0; i < deps.length; i++) {
      args[i] = load(deps[i]);
    }
    return callback.apply(null, args);
  };
})();

