CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers

INTRODUCTION
------------

Hovercard is a module which is based on hovercard, a free light weight jQuery
plugin that enables you to display related information with the hovered label,
link, or any html element of your choice. This module extends Drupal to provide
Hovercard for the users of the website.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/node/2308117

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/node/2308911


REQUIREMENTS
------------

 * Libraries Module - 2.x
   https://www.drupal.org/project/libraries

 * jQuery Hovercard Plugin - v1.0
   https://github.com/prashantchaudhary/hovercard/archive/master.zip
   Please visit https://github.com/prashantchaudhary/hovercard/ for jQuery
   Hovercard Plugin's page.


INSTALLATION
------------

 * The hover_card.module is very similar to other Drupal modules which requires
   Libraries Module to use 3rd party code integration. Hence, for installation
   of the Hover Card module please follow the below mentioned steps:

 * Install as usual, see https://goo.gl/OTdd9P for further information.

 * Download and install the Libraries Module - 2.x from
   https://www.drupal.org/project/libraries.

 * Download the compressed version of jQuery Hovercard Plugin from
   https://github.com/prashantchaudhary/hovercard/archive/master.zip extract the
   files jquery.hovercard.js and jquery.hovercard.min.js into
   sites/all/libraries/hover_card/ 

 * Now, in your sites/all/modules/contrib/ directory download the Hover Card
   module.

 * Enable the Hover Card module.


CONFIGURATION
-------------

 * After enabling it please check your admin/reports/status where there should
   be a new option showing Hover Card Plugin - v1.0 installed with a success
   (green) status.

 * There is configuration link for this which you can access at
   admin/config/people/hover-card. When enabled and configured properly, this
   module will display the hover card to the user links with 'username' as class
   to their anchor tags. To disable the hover card from user links, disable the
   module and clear caches.


MAINTAINERS
-----------
Current maintainers:
 * Rishi B. Kulshreshtha - https://www.drupal.org/user/1403808
