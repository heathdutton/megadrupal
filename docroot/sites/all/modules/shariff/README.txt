
                                   ()
  ┌───────┐                        /\
  │       │                   ()--'  '--()
  │  a:o  │  acolono.com        `.    .'       Shariff Module
  │       │                      / .. \
  └───────┘                     ()'  '()


This module implements the Shariff sharing buttons by heise online:
https://github.com/heiseonline/shariff

Shariff enables website users to share their favorite content without
compromising their privacy.

It consists of two parts: a simple JavaScript client library and an
optional server-side component. The latter fetches the number of likes,
tweets and plus-ones.

The base shariff Drupal module implements the JavaScript library to
display the buttons as a block and a panels pane.


-- REQUIREMENTS --

* Libraries API module
  http://drupal.org/project/libraries
  
* Shariff Library (at least v1.4.6)
  https://github.com/heiseonline/shariff

* JQuery Update module
  http://drupal.org/project/jquery_update
  Shariff requires jQuery 1.7 or higher. Please configure jQuery Update
  accordingly.


-- INSTALLATION --

1) Download the library from https://github.com/heiseonline/shariff and place
it in your libraries folder.
When your libraries folder is "sites/all/libraries" the JavaScript and the
CSS files should be available under
"sites/all/libraries/shariff/build/shariff.min.js",
"sites/all/libraries/shariff/build/shariff.min.css" and
"sites/all/libraries/shariff/build/shariff.complete.css".
You only need the build folder and at least v1.4.6 of the library.

When you have drush you can use "drush shariff-library" or "drush shariff"
to download the library into your "sites/all/libraries" folder. If needed
specify another libraries path as argument:
E.g. "drush shariff sites/default/libraries"


2) Activate the module.

3) Set your default settings under /admin/config/services/shariff. When you
have Font Awesome already loaded on your site be sure to choose the Minimal
CSS option (so that shariff.min.css without Font Awesome will be loaded).

4) Now you can place the buttons as a block, a field or as a panels pane.
You find the panels pane in the panels content administration UI
under Widgets.
