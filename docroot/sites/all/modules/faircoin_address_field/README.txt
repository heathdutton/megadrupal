CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------
This module creates a new faircoin address field type, that is
available to attach to any entity. When user introduces the
faircoin address, the module validates it. When the field is
displayed, it presents qr code next to the address.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/xavip/2539964

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2539964

REQUIREMENTS
------------
This module requires the following modules:

 * Libraries API 2 (https://drupal.org/project/libraries)

This module requires the following js libraries:

 * jquery.qrcode
   Download: https://github.com/XaviP/jquery-qrcode/archive/master.zip
   Author jeromeetienne: https://github.com/jeromeetienne

INSTALLATION
------------
 * Download and install the Libraries API 2 module.

 * Download the qrcode.js plugin from
   https://github.com/XaviP/jquery-qrcode/archive/master.zip

 * Unpack and rename the plugin directory to "qrcode" and place it
  inside the "sites/all/libraries" directory. Make sure the path
  to the plugin file becomes:
  "sites/all/libraries/qrcode/jquery.qrcode.min.js"

 * Enable Faircoin address field module.

CONFIGURATION
-------------
Once the module is enabled, the new type of field "Faircoin address"
is available to attach to content types, user profile, entities,...

MAINTAINERS
-----------
Current maintainers:
 * Xavi Paniello (XaviP) - https://www.drupal.org/u/xavip
