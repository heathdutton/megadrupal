Drupal domwindow module:
------------------------
Maintainers:
  Rob Montero (dgo.to/@rmontero / http://drupal.org/user/191552)
Requires - Drupal 7
License - GPL (see LICENSE)


Overview:
--------
DOMWindow is a lightweight customizable lightbox plugin for jQuery.
This module allows for integration of DOMWindow into Drupal.
The jQuery library is a part of Drupal since version 5+.

Images, forms, iframed or inline content etc. can be displayed in a
overlay above the current page.

* jQuery - http://jquery.com/
* DOMWindow - http://swip.codylindley.com/DOMWindowDemo.html


Features:
---------

* Drush command to download and install the DOMWindow plugin in
  sites/all/libraries

The DOMWindow plugin:

* Supports displaying inline divs, iframes and ajax loaded markup on a DOM 
  overlay window.
* Lightweight.
* Robust API with plenty methods to control and tweak the overlay experience.
* Released under the MIT License.


Installation:
------------
1. Download the DOMWindow plugin in "sites/all/libraries/jquery.domwindow/".
   Link: http://swip.codylindley.com/jquery.DOMWindow.js
   Drush users can use the command "drush domwindow-plugin".
2. Download and unpack the DOMWindow module directory in your modules folder
   (this will usually be "sites/all/modules/").
3. Go to "Administer" -> "Modules" and enable the module.

Drush:
------
A Drush command is provides for easy installation of the DOMWindow
plugin itself.

% drush domwindow-plugin

The command will download the plugin and unpack it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not
recommended unless you know what you are doing.

Roadmap:
--------

Provide tighter integration with core Drupal and an easier user experience by 
implementing functionality like that of the colorbox module.

Change Log:
----------
01/14/2012 rmontero Initial commit.

Last updated:
-------------
01/14/2012