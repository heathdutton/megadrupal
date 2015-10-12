
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Example
 * Extending
 * Installation


INTRODUCTION
------------

Current Maintainer: PDNagilum <pdnagilum@gmail.com>

Ever needed to call a Drupal function from JavaScript that isn't already
available in the Drupal JavaScript object? Javascript Drupal Extension fixes
that by extending the Drupal object to expose PHP functions via AJAX-calls.

By default this module includes drupal_get_path() and watchdog(). It all
started with me needing to know the path of a module from JavaScript. I wrote
a much simpler version that what you have in front of you now, and thus
Javascript Drupal Extension was born. The latter function was added after a
while, as it too is really nice to have around during development as well as
debugging.


EXAMPLE
-------

var path = Drupal.drupal_get_path('module', 'my-awesome-module');

-or-

Drupal.drupal_get_path(
  'module',
  'my-awesome-module',
  'callback-function-for-path');

Both these queues up the AJAX call, which by default is filtered through a
whitelist to ensure safety, and either returns the output of the called
function directly, or sends the output to the callback function supplied as
the last argument. You can also supply another callback after the first one
which will be used in case the AJAX-call fails for some reason. The default
action in that case is to throw an error.


EXTENDING
---------

The module is written in such a way that extending other functions is as easy
as adding a few lines of codes.

Let's say you wish to extend the t() function out to JavaScript. Yes I know it
is already in the Drupal object, but just for show, let's pretend it isn't.

All you have to do is add the following JavaScript code, somewhere:

Drupal.t = function (text) {
  return javascript_drupal_extension_ajax_call('t', { text });
}

If you have the whitelisting feature enabled (which it is by default), you
would also have to add the 't' function in the admin section.

If you want to throw callbacks in the mix, just extend the code as such:

Drupal.t = function (text, callback_success, callback_failure) {
  return javascript_drupal_extension_ajax_call(
    't',
    { text },
    callback_success, callback_failure);
}


INSTALLATION
------------

Installation is pretty streight forward and should require no code alterations
by the developer, unless you want to add more functions to call.

The module sets three variables in the Drupal variable-set which are also
deleted upon uninstall.

There is a chance that the AJAX call won't work right after install, which is
because hook_menu() function declares the AJAX URL that's needed for calling.
If you get a 404 from the call, just clear the cache and you're good.
