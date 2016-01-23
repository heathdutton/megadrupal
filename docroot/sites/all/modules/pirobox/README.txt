The Pirobox module comes with 2 jQuery plugins:

  - Pirobox jQuery plugin
    is based on the Pirobox jQuery plugin (http://www.pirolab.it/pirobox).
  - Blur gaussian jQuery plugin
    is based on jQuery Images Blur plugin
    (https://sites.google.com/site/rainerrillke/dev/js/blur)

The module uses a modified version of the Pirobox jQuery plugin (not the
extended version).
This makes it possible to
  - use Drupal like translation of the Pirobox,
  - configure certain settings of the Pirobox,
  - use the Gallery covering feature,
  - use the Blur gaussian jQuery plugin.

The module uses a modified version of the jQuery Images Blur plugin.
This makes it possible to
  - use a callback function.

INSTALLATION
------------
1. Copy the extracted Pirobox folder to modules directory or install the module
   with the UI at admin/modules/install.
2. I M P O R T A N T
   Move all content from the Pirobox module folder pirobox/move_to_libraries
   to sites/all/libraries.
   Please see the section PIROBOX LIBRARY below.
3. At admin/build/modules enable the Pirobox module and its contained modules.
4. Administer the Pirobox module permissions at admin/people/permissions.
5. Configure the Pirobox module at admin/config/media/pirobox.

BLUR GAUSSIAN
-------------
The Blur gaussian jQuery plugin enables to use a facebook like image effect
in the lightbox.
Enable/disable and configure this effect at
admin/config/media/pirobox -> Lightbox image effects.

LIMITATIONS
------------
Pirobox supports only images.

ADDITIONAL MODULES
------------------
The Pirobox Limit module (drupal.org/project/pirobox_limit).
The Extend image module (drupal.org/project/eim).

PIROBOX LIBRARY
---------------
The 2 jQuery plugins exist in the module folder pirobox/move_to_libraries.
Move this plugins to the sites/all/libraries folder.

Plugin content in the libraries folder sites/all/libraries/pirobox/

  Pirobox library folder structure
    libraries/pirobox/css/
    libraries/pirobox/images-background/
    libraries/pirobox/js/

  Styles
    libraries/pirobox/css/demo1/
    libraries/pirobox/css/demo2/
    libraries/pirobox/css/demo3/
    libraries/pirobox/css/demo4/
    libraries/pirobox/css/demo5/
    libraries/pirobox/css/Fancy_1

    This folder contains the styles code files.
    The main file is the file piroboxstyle.css.

    Feel free to use own styles. A pure CSS style must contain at least
    the file piroboxstyle.css.

  Overlay background images
    libraries/pirobox/images-background/Black-Grey-Lines.gif
    libraries/pirobox/images-background/Darkgrey-Squared.jpg
    libraries/pirobox/images-background/Grey-Diagonal-Lines-2.png
    libraries/pirobox/images-background/Grey-Diagonal-Lines.png
    libraries/pirobox/images-background/Grey-Squared.jpg

    Feel free to use your own images.
    Considered extensions are 'png', 'gif', 'jpg' and 'jpeg' (case insensive).

  Pirobox jQuery JS files
    libraries/pirobox/js/pirobox.js
    libraries/pirobox/js/pirobox.min.js

Plugin content in the libraries folder sites/all/libraries/blur-gaussian/

  Blur gaussian jQuery JS files
    libraries/blur-gaussian/blur-gaussian.js
    libraries/blur-gaussian/blur-gaussian.min.js
