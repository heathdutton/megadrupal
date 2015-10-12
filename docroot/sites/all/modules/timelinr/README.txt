Drupal timelinr module:
-----------------------
Maintainers:
  SteffenR (http://drupal.org/user/244597)
  bfr (http://drupal.org/user/369262)
Requires - Drupal 7

Overview:
---------
Timelinr is a customizable content-slider plugin for jQuery 1.4.4+. 
The module allows the integration of timeline into drupal.
The jQuery Plugin can be found here:
http://www.csslab.cl/2011/08/18/jquery-timelinr/

Features:
---------

The views Plugin:

* Provides a views display-style that integrates all options provided by jQuery plugin.
* Provides a views row-plugin, where you can add the fields of the view.
* Style of timelinr is fully customizable via CSS or custom template or theme-function.
* Integration for image-fields

The jQuery plugin:

jQuery Timelinr is a simple plugin helps you to give more life to the boring timelines. 
It supports horizontal and vertical layouts, and you can specify parameters for most attributes: speed, transparency, etc.


Installation: 
-------------
Follow instructions in INSTALL.txt.

Usage:
------
01) Create a new view and choose "Views Timelinr" as display format.
02) Add some fields that you want to show in the timelinr view - you need at least a field for date and a title field
All settings done here will be used in the "Views-Timelinr" row-style.
03) Switch "Row style options" to "Views Timelinr" and assign the fields you choosed previously.
For the date field a field shorter than 10 characters (date or year) is recommended - it'll be displayed as the navigation of time-slider.

Styling:
-------
The jQuery timeline plugin comes with inbuilt styles, that include some unwanted animations/ text-styling.
These styles can be resetted by setting the option 'reset styles' in the display-format plugin.