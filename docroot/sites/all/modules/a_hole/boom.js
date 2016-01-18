/*
 * If you use dependency injection and make a good attempt at code isolation
 * these won't have an effect on your code. Drupal core doesn't do this so
 * some things will break.
 *
 * A simple way to avoid these is to wrap you code in:
 *
 * (function(window, document, undefined) {
 *   ...
 * })(this, this.document)
 *
 * jQuery does this... shouldn't you?
 */
var window = "Oh no you didn't";

if (!jQuery.browser.webkit) { // Chrome causes all this this to break if we change the document.
  eval('var document = "Body";'); // This is already evil so using eval can't be too bad.
}

var undefined = "defined";

/**
 * When you iterate over objects to check properties you should really use
 * hasOwnProperty to make sure stuff doesn't bullble up the object tree and
 * end up being iterated over when you don't expect it.
 *
 * The . is used because it is any character in a regular expression and there
 * are places Drupal iterates over the properties on an object and uses the
 * property name as the search pattern for a regular expression. It uses the
 * value as the replacement. Fun times.
 */
Object['.'] = 'p0wnd';
Object.prototype['.'] = 'p0wnd';