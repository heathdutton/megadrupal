CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

The Image Styles Info module provides page to show the image styles with
preview and additional info. Page is available by the following path:
/admin/reports/image-styles-list


REQUIREMENTS
------------
This module requires the Image module (available in core).


INSTALLATION
------------
Install as you would normally install a contributed Drupal module.
See: https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------
Access to Image Styles List page could be configured by the "Administer image
styles" access permission. There is no any additional configuration.

Sample image (to generate preview images) can be changed using
"image_styles_info_sample" variable.
For example:
$conf['image_styles_info_sample'] = 'sites/default/files/sample.jpg';

Otherwise, the default image (path_module/image_styles_info/images/sample.jpg)
will be used.


MAINTAINERS
-----------
Current maintainers:
 * Pavel Shevchuk (pshevchuk) - https://www.drupal.org/u/pshevchuk

