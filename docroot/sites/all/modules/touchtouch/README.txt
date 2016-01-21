
INTRODUCTION
------------
TouchTouch is a module written by Blacksnipe for Drupal 7.x to enable
a front-end gallery for images in fields, based on an
extended(*) version of the jQuery-plugin by Martin Angelov described on
http://tutorialzine.com/2012/04/mobile-touch-gallery/.

You can add it as a field formatter or simply use the formatter for
fields in Views.

(*) The plugin is based on the one by Martin Angelov, but with some
modifications like:
  - The popup now has an extra button to close the overlay.
  - When pressing ESC on the keyboard, the overlay closes.
This is the main reason why the plugin is not loaded as a library!


REQUIREMENTS
------------
None. There is support for the Views-module and the Masonry-module, 
but these are not required


INSTALLATION & CONFIGURATION
----------------------------

* Add as field formatter
------------------------
1. Download the library from 
   https://github.com/Blacksnipe/touchTouch/archive/master.zip 
   and add the /assets/touchTouch folder to the libraries. Rename 
   the folder from 'touchTouch' to 'touchtouch' (the main file 
   should be under sites/all/libraries/touchtouch/touchTouch.jquery.js)
2. Enable the module and the dependencies.
3. Edit the content type which contains the field you want to show as the
   TouchTouch-gallery.
4. Manage the display of the content type
5. Change the format of the Image-field you'd like. Change it to
   "TouchTouch gallery".
6. Select an image style for the clickable preview image
   ... and the big image (not required).
7. Save the settings.
8. That's it!


* Use in Views
--------------
1. Download the library from 
   https://github.com/Blacksnipe/touchTouch/archive/master.zip 
   and add the /assets/touchTouch folder to the libraries. Rename 
   the folder from 'touchTouch' to 'touchtouch' (the main file 
   should be under sites/all/libraries/touchtouch/touchTouch.jquery.js)
2. Enable the module and the dependencies.
3. Add the fields or render the entities and follow the steps under "Add as
   field formatter".
4. When selected as fields, set the formatter of the field to
   "TouchTouch gallery".
5. Select an image style for the clickable preview image
   ... and the big image (not required).
6. Save the view.
7. That's it!


Module project page:
  https://www.drupal.org/sandbox/blacksnipe/2419583

To submit bug reports and feature suggestions, or to track changes:
  http://www.drupal.org/project/issues/2419583


TROUBLESHOOTING
---------------

To submit bug reports and feature suggestions:
  http://www.drupal.org/project/issues/2419583


FAQ
---

So far: none.


MAINTAINERS
-----------

If you'd like to complain, or - preferably - give suggestions, feel free to
contact these nice folks:
- Blacksnipe - http://drupal.org/user/526204
