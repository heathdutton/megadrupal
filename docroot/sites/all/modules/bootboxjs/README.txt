Maintainers:
  Kevin Yousef (https://drupal.org/user/1229220)
Requires - Drupal 7
License - GPL (see LICENSE)

-- OVERVIEW --
This module loads Bootbox.js via Libraries API, regardless of the theme you are using.
It does nothing else.

See the documentation of Bootbox.js for more information on how to use the library.
http://bootboxjs.com/

-- REQUIREMENTS --
Bootstrap v3.x with modal.js.

-- INSTALLATION --

1. Download bootbox.min.js file and place it on /sites/all/libraries/bootboxjs or use drush bootboxjs-plugin.
2. Install and enable the module.

-- USAGE --
In your module code, add the library with libraries_load() and drupal_add_library().
If unsure where, add it in an implementation of hook_init().
Use 'bootboxjs' for both the module and library name arguments, like this:
libraries_load('bootboxjs');
drupal_add_library('bootboxjs', 'bootboxjs');

-- DRUSH --
A Drush command is provides for easy installation of the Bootbox.js
plugin itself.

% drush bootboxjs-plugin

The command will download the plugin and place it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not
recommended unless you know what you are doing.

-- CONFIGURATION --
None.
