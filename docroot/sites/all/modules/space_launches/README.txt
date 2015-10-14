CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation

INTRODUCTION
------------

This module provides a way to retrieve, store, and display upcoming space
launches.

The core module provides an entity, cron task and hook that modules can
implement to provide sources for space launch schedules. Included by default
is the Space Launches NASA module, which adds NASA as a source.

REQUIREMENTS
------------

This module and its sub-modules require the following:

 * Entity API (https://www.drupal.org/project/entity)
 * Libraries API (https://www.drupal.org/project/libraries)

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * Download the NASA Web Data library.
   https://github.com/ruscoe/NASA-Web-Data-PHP/releases/tag/v1.0

 * Extract the NASA Web Data library to your libraries directory, so that the
   library files are located at: libraries/NASA-Web-Data-PHP/
   The path is important.

   See: https://www.drupal.org/project/libraries for further information about
   using libraries.

 * Enable the included Space Launches NASA (space_launches_nasa) sub-module to
   add a space launch schedule source.

 * Run cron manually to import some upcoming space launches.
   See: https://www.drupal.org/node/158922 for further information on cron.

 * Add the "Upcoming space launches" block to a region of your site to see the
   imported launches displayed in a nice list.
