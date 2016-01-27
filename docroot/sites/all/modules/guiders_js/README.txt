CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Live Examples
 * Requirements
 * Installation
 * Support
 * Compatibility
 * Authors

INTRODUCTION
------------

The Guiders-JS module uses the guiders_js.js (https://github.com/jeff-optimizely/Guiders-JS) javascript library to allow a content editor to create guiders

For more information see: https://github.com/jeff-optimizely/Guiders-JS


LIVE EXAMPLES
-------------

Here are a couple examples hosted online.

http://jeffpickhardt.com/guiders/

https://optimizely.appspot.com/edit#url=www.google.com

REQUIREMENTS
------------

* guiders.js library (1.2.6+)

* Libraries module - http://drupal.org/project/libraries

INSTALLATION
------------

1. Copy the guiders_js module directory to your sites/all/modules or sites/all/modules/contrib directory.

2. * Create a directory "guiders-js" inside the path "sites/all/libraries/"

   * Download the guiders.js library from:

     https://github.com/jeff-optimizely/Guiders-JS

   * Extract the files to sites/all/libraries/guiders-js

   * Leave only the guiders-1.2.6.js, guiders-1.2.6.css and x_close_button.jpg files, remove the others

   * Rename the file guiders-1.2.6.js to guiders.js

   * Rename the file guiders-1.2.6.css to guiders.css

   * The actual guiders.js file should be located in:

     /sites/all/libraries/guiders-js/guiders.js

3. Enable the module at Administer >> Site building >> Modules.

4. Visit admin/content/guiders/packs to create your first guider's pack

5. That's it!


SUPPORT
-------

If you find a bug or have a feature request please file an issue: http://drupal.org/node/add/project-issue/1227788

COMPATIBILITY
-------------

A lot of work has gone into ensuring maximum compatibility with other contrib modules. If you find a bug please use the issue tracker for support. Thanks!

AUTHORS
--------

* Ehud Shahak (basik.drupal) - http://drupal.org/user/134140 & http://ehudshahak.com
