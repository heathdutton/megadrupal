
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Customization
 * Design Decisions


INTRODUCTION
------------

Current Maintainers:
MrMaksimize (http://drupal.org/user/801596)
iler (http://drupal.org/user/726092)

Noty Messages is a library that brings in cuztomization available in the noty
jQuery plugin (http://needim.github.com/noty/) into drupal, and allows the the
library's rendering of messages overtake Drupal's default message theming style.
The goal of the module is be extremely flexible and allow overriding of only
certain Drupal message types if that's what the user wishes.

If you would like to test out the library, visit http://needim.github.com/noty/


INSTALLATION
------------

The Noty Messages module has 3 dependencies,
jQuery Update (http://drupal.org/project/jquery_update)
Libraries module (http://drupal.org/project/libraries)
Noty jQuery Plugin (https://github.com/needim/noty/).

1. Install and enable Libraries module and jQuery Update module.
2. Make a directory in sites/all/ (or for multisite the site name) called
   libraries.
3. Visit https://github.com/needim/noty and clone the master branch into your
   libraries folder.
4. The path of the jquery.noty.js file should be
   sites/all/libraries/noty/js/jquery.noty.js (or adjusted for multisite)
5. Visit admin/config/user-interface/noty-messages to adjust any needed
   configurations.
6. You're good to go!


CUSTOMIZATION
-------------

Customization:

There are several built in themes inside the noty library.  You can use those or
you can override them just like you would any normal css file.

If you choose custom, you have to make a file in the directory
your_theme/css/noty_theme_custom.css and write your own theme if you choose
to do so.

In admin, all themes are getting loaded for preview including custom,
however any css customization you choose to make to the default themes will not
show up in admin preview.

DESIGN DECISIONS
----------------

While there are no hooks currently implemented, the module was designed in such
a way that it would allow for pretty specific hooking if needed.
So if you would see a need for a hook somewhere, submit an issue or a patch!
