INTRODUCTION
------------
this module provide field display fomate as
360 degree sildeshow/image rotate
with power of jquery.

REQUIREMENTS
------------
This module requires the following modules:
* Libraries (https://www.drupal.org/project/libraries)

This module requires the following javascript libraries:
* drupal_threesixty_slider
  (https://github.com/code-rider/drupal_threesixty_slider)

Recommended modules
-------------------
There are several modules that can be used to bulk upload images.
One that I've tested positivly.
* Multiupload Imagefield Widget
  (https://www.drupal.org/project/multiupload_imagefield_widget)

INSTALLATION
------------
1. Download javascript library from
   https://github.com/code-rider/drupal_threesixty_slider
   and Extract the contents of this library into sites/all/libraries
   and rename tha library 
   drupal_threesixty_slider-1.x to drupal_threesixty_slider.
2. download and Extract required module Libraries API into
   sites/all/modules and enable
3. Extract the contents of this project/module into
   sites/all/modules/ and enable

CONFIGURATION
-------------
You must have a content type with a field of type "Image" with the possibility
to upload an unlimited number of images.

now go for display settings and select "Slideshow j360"
for your field and set height and width as your theme requiered.
and select Display from Navigation Display
if wants display navigation else select Not Display.

you are done.

TROUBLESHOOTING
---------------
This functionality/module required a set of sequins images.
If you see your images rotate opposite side of navigation
then Edite you node and change you images upload sequins.
like

Before:
image_1 ,
image_2 ,
image_3 ,
image_4 ,
image_5

After:
image_5 ,
image_4 ,
image_3 ,
image_2 ,
image_1

MAINTAINERS
-----------
Current maintainers:
* Rashid Abdullah (coderider) - https://www.drupal.org/u/coderider
