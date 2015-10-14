RoyalSlider
===============
This module provides integration with the excellent RoyalSlider library.

It contains the RoyalSlider module and an implementation for views in the Views Slideshow: RoyalSlider module.

Features of the RoyalSlider module:
 - Configuration stored in Features-exportable "Option Sets" (based on FlexSlider)
 - Field formatter for image fields
 - Views Slideshow integration
 - Full support for custom RoyalSlider Skins
 - API functions to manually include the RoyalSlider library and/or option sets and/or skins

Installation
=============

1) Upload/install the Libraries API module. http://drupal.org/project/libraries
2) Upload/install the ctools library. http://drupal.org/project/ctools
3) Upload/install the jQuery Update module. https://drupal.org/project/jquery_update
4) Configure jQuery Update to use jQuery 1.7+
5) Create a sites/all/libraries directory on your server.
6) Create a directory within sites/all/libraries named royalslider.
7) Locate/download the RoyalSlider plugin. http://dimsemenov.com/plugins/royal-slider/
8) Upload/install the RoyalSlider plugin: place it inside the royalslider
   directory.
9) Enable the RoyalSlider module
10) You should now see the new views style option called "Slideshow"

For the Views Slideshow: Royalslider:
11) Upload/install the Views 3 module. https://drupal.org/project/views
12) Upload/install the Views Slideshow module. https://drupal.org/project/views_slideshow
13) Enable the Views Slideshow: RoyalSlider module

Requirements
============
* RoyalSlider library
* Libraries
* jQuery Update (jQuery 1.7+)
* Ctools

Views Slideshow: RoyalSlider
* Views 3
* Views Slideshow


Description
===========

The RoyalSlider module will provide a new field display option for image fields: RoyalSlider.
If you select the display option, you can select which "Option Set" you want to use for that field.
You can configure Option Sets at 'admin/config/media/royalslider'.
For more information about the different options, see the documentation of RoyalSlider: http://dimsemenov.com/plugins/royal-slider/documentation/

For more information about the usage of the Views Slideshow: RoyalSlider module, read the README.txt in its own folder.


Authors/maintainers
===================

Original Author:

Alex Weber
http://drupal.org/user/850856

Co-maintainers:

thijsvdanker
http://drupal.org/user/234472/dashboard


Support
=======

Issues should be posted in the issue queue on drupal.org:

https://drupal.org/project/issues/royalslider