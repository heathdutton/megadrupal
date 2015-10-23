Mediabox - Universal Image Library
==================================

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Functionality
 * Modules
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting & FAQ
 * Maintainers

INTRODUCTION
------------
Mediabox is a universal image library which out of the box provides easy to use
UI, aggregated control of library images, inline cropping and extended
flexibility of the data model.
 * For a full description of the module, visit the project page:
  https://drupal.org/project/mediabox
 * To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/mediabox

FUNCTIONALITY
-------------
 * Field widget
 * Just In Time - cropping of images from/to desired image style in context of widget
 * Easy to use library - creating, editing, removing, selecting an existing
(multiple) images in the context of a widget
 * Bulk upload of images using plupload
 * Library display
 * Integration of library, and administration with views
 * Text Filter integration - inserting token in text which will be replaced by
actual view of a Mediabox Display
 * Integration of formatter with views. This option allows you to create gallery
displays accommodating various needs. ( Views slide show plugins, etc. )

MODULES
-------
Project mediabox contains 4 modules:
 * mediabox
 * mediabox_ui
 * mediabox_library
 * mediabox_crop

REQUIREMENTS
------------
This module requires having those following modules:
 * Views (https://drupal.org/project/views)
 * Views Bulk Operations (VBO) (https://drupal.org/project/views_bulk_operations)
 * Entity (https://drupal.org/project/entity)
 * Chaos tool suite (ctools) (https://drupal.org/project/ctools)

Additional module that is required for mediabox_ui module:
 * Plupload integration (https://drupal.org/project/plupload)

Additional libraries should be downloaded:
 * Jcrop (https://github.com/tapmodo/Jcrop/archive/v0.9.12.tar.gz)
Notes about Jcrop:
 * Manually remove folder Jcrop/demos/ for security reasons!
 * Tested for versions 0.9.9 - 0.9.12
 * Jcrop library default location is:  sites/all/libraries/Jcrop

Modules that are required to be enabled for simple usage of mediabox module:
 * mediabox, views_bulk_operations, ctools, views, entity
Additional modules that are required to be enabled for complete usage of mediabox module:
 * mediabox_ui, mediabox_crop, mediabox_library, plupload

INSTALLATION
------------
Recommended way of installing is by using drush (https://drupal.org/node/1791676)
Commands that needs to be executed:

drush dl mediabox
drush en mediabox
drush en mediabox_ui
drush mediabox-jcrop

All external dependencies will be installed if using this approach,
otherwise please refer to REQUIREMENTS section in order not to miss some.

CONFIGURATION
-------------

Module mediabox defines the following permissions:
 * access mediabox library
 * upload mediabox files
 * manage mediabox files
 * administer mediabox types
 * administer mediabox settings
Module mediabox_ui defines the following permissions:
 * administer media display types

TROUBLESHOOTING & FAQ
---------------------

 * Mediabox isn't working properly:
  - Have you confirmed that you met all requirements in REQUIREMENTS section?
 * Mediabox is working but images are not saved:
  - Have you checked permission for sites/default/files?
  - Make sure that folder is writable.

MAINTAINERS
-----------

Current maintainers:
 * Ivica Puljic (pivica) - https://drupal.org/user/41070
 * Vladan Djokic (vladan.me) - https://drupal.org/user/2640361
 * Vladimir Mitrovic (devlada) - https://drupal.org/user/1663518
 * Erik Vinclav (e.vinclav) - https://drupal.org/user/1044890

Mediabox is fork of xmedia originally developed
(and continues to develop this fork) by e.vinclav.
