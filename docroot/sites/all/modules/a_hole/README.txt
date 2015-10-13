This module is designed to exploit bad development practices. If you're code
uses dependency injection and properly isolates itself from the outside scripts
this module won't make a difference. Unfortunately there is a lot of JavaScript
in Drupal core that doesn't do this so stuff will break in some browsers just
by enabling this module.

The dependency injection part can be solved by wrapping your code in something
like:

(function($, window, document, undefined) {
  ...
})(jQuery, this, this.document)

This wrapper is a self executing function which passes in jQuery, the global
object (window and this are the same here), and the document. You'll notice
undefined has nothing passed in for it. This makes sure the value is undefined.

This isn't the only want to solve the scoping problem but it is a successful way.

Another thing we often do is iterate over the properties in an object. If we
don't check that the property is one for our current object you may end up with
properties defined up the object inheritance chain popping up in our loops. For
example:

Object.prototype.foo = 'bar';

var hot = {
  e: 'ieio',
  boom: 'stick'
};

for (var burn in hot) {
  if (hot.hasOwnProperty(burn)) {
    alert(hot[burn]);
  }
}

'bar' would be one of the items alerted to us if we didn't have the wrapper of:

if (hot.hasOwnProperty(burn)) {
  ...
}

This is because it is up the inheritance chain for objects.