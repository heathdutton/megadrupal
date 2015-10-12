CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------

 * Selfi allows to capture image using WebRTC, which can be used to set profile
   picture.
  - https://www.drupal.org/sandbox/das.gautam27/2437741

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * Selfi module comes with a configuration panel, where you can set the folder
   where the images will be stored.
   Visit admin/config/media/selfi for the configuration options.

HOW TO USE
----------
 1) Goto admin/modules and Install the module selfi.

 2) Configure selfi from /admin/config/media/selfi ,
specify the public folder path where the images will be stored (optional) , By default images will be stored in sites/default/files/selfi_clicks directory.

 3. Please check directory permission. NOTE: Directory should be writable by Drupal.

 4) Visit user profile edit page, Select option "Take picture" in PICTURE section. Click on Start camera button, browser will ask for accessing your webcam.

 5) Click on Take picture button and save your profile.

Note : Selfi use public file system for storing the images.

MAINTAINER
-----------
Current maintainers:
 * Gautam Das (das.gautam27) - https://www.drupal.org/u/das.gautam27
 * Swapnil Bijwe (vb_swapnil) - https://www.drupal.org/u/vb_swapnil
