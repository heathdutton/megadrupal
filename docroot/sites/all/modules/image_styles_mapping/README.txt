CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Similar modules
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

This module allows you to have reports listing the image styles per image
fields and per view modes on all entities.

Additional reports will be provided if the recommended modules are enabled.

The report can be accessed at admin/reports/image_styles_mapping.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/image_styles_mapping
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2310347


REQUIREMENTS
------------

This module requires the following module:
 * Image (Core)

RECOMMENDED MODULES
-------------------

 * Picture (https://www.drupal.org/project/picture):
   When Picture is enabled, add a column in the fields mapping to indicate the
   pictures used.

 * Scald: Atom reference and Scald image (https://www.drupal.org/project/views):
   When Atom reference and Scald image are enabled, add a mapping of the image
   styles through a mapping of the scald context.

 * Views (https://www.drupal.org/project/views):
   When Views is enabled, add a mapping of the image styles used in view fields.
   If Atom reference and Scald image are enabled, add a mapping of the image
   styles through a mapping of the scald context used in view fields.

 * Views UI (part of Views https://www.drupal.org/project/views):
   When Views UI is enabled, in the views mapping, the output will be displayed
   as link to directly edit the views.


SIMILAR MODULES
---------------

 * Style usage (https://www.drupal.org/project/style_usage)


INSTALLATION
------------

 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

The module has no menu or modifiable settings. There is no configuration.


MAINTAINERS
-----------

Current maintainer:
 * Florent Torregrosa (Grimreaper) - https://www.drupal.org/user/2388214
