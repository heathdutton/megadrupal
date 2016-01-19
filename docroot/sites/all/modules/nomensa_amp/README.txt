
CONTENTS OF THIS FILE
---------------------

 * About the Module
 * Dependencies
 * Installation
 * Usage
 * Maintainers

ABOUT THE MODULE
----------------

This module provides integration with Nomensa's Accessible Media Player.

DEPENDENCIES
------------

 * Libraries API (http://drupal.org/project/libraries)

INSTALLATION
------------

 * Download and enable the Libraries API module.
 * Download and enable the Nomensa AMP module.
 * Download the Nomensa Accessible Media Player from GitHub
   (https://github.com/nomensa/Accessible-Media-Player).
 * Place the player within your libraries directory and rename it to
   'nomensa_amp'.

USAGE
-----

How to use the video player:
 * Go to /admin/config/media/nomensa-amp and configure the module as required.
 * Create a node and enter the HTML link to the video.
   * If the setting is enabled, ensure that the "player" class is added to the
     link (Drupal 7 only, as of release 7.x-1.2 - see
     https://www.drupal.org/node/2035311).
 * The video player will automatically be loaded in place of the link.

MAINTAINERS
-----------

Oliver Davies (http://drupal.org/user/381833)
Emily Coward (http://drupal.org/user/231558)
