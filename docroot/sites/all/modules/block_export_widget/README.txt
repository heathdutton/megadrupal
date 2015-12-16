CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers

INTRODUCTION
------------
The Block export widget module allows export arbitrary block as a HTML widget
that could be embedded to a 3-rd party site.
That widget contains only HTML of the block. If block requires CSS or
Javascript - they should be handled separately.
Widget could be embedded directly on the other site page using iframe
(easiest option) or by making cURL request and then outputting on other site.
For each block export could be enabled/disabled and export format could be
configured.
Module implements custom lightweight and fast function to output HTML page,
so full Drupal page output process is not used.

REQUIREMENTS
------------
This module requires the following modules:
 * Block

RECOMMENDED MODULES
-------------------
 * Boxes (https://www.drupal.org/project/boxes):
   When enabled, allows to export content of the boxes.

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * User must have "Administer blocks" permission to configure block export.
 * Configure the block export settings in Administration » Configuration »
   Web services » Block export widget.
 * For each block export could be enabled and proper export format selected.
 * Use "export" link to access block export widget.

TROUBLESHOOTING
---------------
You should assume that request from the 3rd party site will be handled as
request from anonymous user, so exported widget will contain content that is
accessible for anonymous users only.
If requested block doesn't exist or export is not enabled for it,
then 404 error will be returned.
If site is in maintenance mode, then 403 error is returned.
Both for 404 and 403 errors empty content is returned.

MAINTAINERS
-----------
Current maintainers:
 * Eugene Fidelin (eugef) - https://www.drupal.org/u/eugene-fidelin
This project has been sponsored by:
 * VNU Vacature media
