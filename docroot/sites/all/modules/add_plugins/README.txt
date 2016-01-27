CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers


INTRODUCTION
------------

Do you ever find yourself creating a custom utility module just for adding you
handy list of common ctools plugins (such as layouts or content types)? Ever
find yourself using the same custom layouts on every site, or wanting to
quickly add in some panels content types without having to manually go and
create a module all over again?

This module helps with your efficiency and sanity by looking for and adding
plugins from [drupal root]/sites/all/plugins. From there, it will follow the
common naming convention within ctools and look for the name of each plugin
type within that folder.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/add_plugins
 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/add_plugins


REQUIREMENTS
------------

This module requires the Chaos Tools Suite (https://drupal.org/project/ctools)
in order for this module to add
functionality.


INSTALLATION
------------

Install as usual, see
https://drupal.org/documentation/install/modules-themes/modules-7 for
further information.


CONFIGURATION
-------------

There is no UI for this module. In order to use this module, you must create
a plugins folder in one of the standard locations:

 * /sites/all/plugins
 * /sites/example.com/plugins
 * /plugins (not a best practice, but it works)

After creating the plugins folder, create a folder for each type of plugin
you're going to be adding (ie. content_types, layouts, etc).

Example steps to add a custom layout:
 1. Enable the module.
 2. Create a plugins folder: /sites/all/plugins.
 3. Create a folder for plugins of type 'layouts': /sites/all/plugins/layouts
 4. Put all of your custom layout within that folder.
 5. Flush your cache.


TROUBLESHOOTING
---------------

It's important that the plugin folders be named exactly what the name of the
type of ctools plugin it is.


MAINTAINERS
-----------

The current maintainer of this module is David Needham
(https://drupal.org/user/191261)