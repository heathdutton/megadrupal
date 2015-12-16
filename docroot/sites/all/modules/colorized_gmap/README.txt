CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

Colorized gmap module allows to add a google map on the site as a drupal block and customize it.
This module expands the standard UI and allows to create a block. At the colorized gmap block creation page you are able to customize a standard google map (e.g. to colorize water, landscape , etc.) You will see changes on the map after every action.

Features:
 * colorize any elements of the map
 * hide unnecessary map controls
 * change map controls position
 * customize marker image and caption
 * add multiple blocks
 * exportable via 'Features' module


 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/colorized_gmap


 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/colorized_gmap


REQUIREMENTS
------------

This module requires the following modules and libraries:
 * Entity API (https://www.drupal.org/project/entity)
 * Libraries API (https://www.drupal.org/project/libraries)


INSTALLATION
------------

 * Download colorpicker (By Stefan Petre) library
   http://www.eyecon.ro/colorpicker and put its content to
   "colorpicker" directory inside libraries directory
   (usually sites/all/libraries) so you should have
   sites/all/libraries/colorpicker directory and js/colorpicker.js and
   css/colorpicker.css in it.
   See https://www.drupal.org/node/1440066 for more details.

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Visit 'admin/structure/block/colorized_gmap_block' to create colorized gmap block.
 * To delete colorized gmap block visit 'admin/structure/block'


MAINTAINERS
-----------

Current maintainers:
 * Artyom Zenkovets (azenkovets) - https://www.drupal.org/u/azenkovets
 * Anton Nepsha (omsk_nepshaaa) - https://www.drupal.org/u/omsk_nepshaaa

This project is created by ADCI Solutions team (http://drupal.org/node/1542952).
