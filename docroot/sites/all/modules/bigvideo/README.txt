CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Warning
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting

INTRODUCTION
------------
The BigVideo module provide the ability for attach background video to site pages.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/bigvideo


 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/bigvideo

WARNING
-------
You'll probably need to adapt/update your theme styles to make your theme look good with background videos.

REQUIREMENTS
------------
This module requires the following modules and libraries:

 * Modules
  - Libraries (https://drupal.org/project/libraries)
  - Video.js (https://www.drupal.org/project/videojs)
  - jQuery Update (https://www.drupal.org/project/jquery_update)

 * Libraries
  - BigVideo.js (https://github.com/dfcb/BigVideo.js)
  - ImagesLoaded (https://github.com/desandro/imagesloaded)

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
 * Make sure what you have installed and configured Video.js module.
 * Download the BigVideo.js and ImagesLoaded libraries.
 * Place the libraries in the appropriate directories
   sites/all/libraries/imagesloaded/imagesloaded.pkgd.min.js
   sites/all/libraries/bigvideojs/css/bigvideo.css
   sites/all/libraries/bigvideojs/css/bigvideo.png
   sites/all/libraries/bigvideojs/lib/bigvideo.js
   sites/all/libraries/bigvideojs/bower.json (if exists)
   sites/all/libraries/bigvideojs/BigVideo.jquery.json (if exists)

CONFIGURATION
-------------
 * Add new BigVideo source at admin/config/user-interface/bigvideo/sources
 * Add BigVideo pages at admin/config/user-interface/bigvideo
 * (extra) BigVideo provide reaction (BigVideo Background) for Context module.

TROUBLESHOOTING
---------------
 * Please change site jQuery version to 1.7 (or newer), this is required for BigVideo library.