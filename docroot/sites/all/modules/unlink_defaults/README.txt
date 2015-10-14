CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * API
 * Maintainers


INTRODUCTION
------------
This modules provides possibility to move site configuration from files to
database with just a couple clicks!

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/unlink_defaults

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/unlink_defaults


REQUIREMENTS
------------
This module requires the following modules:
 * Chaos tool suite (ctools) (https://drupal.org/project/ctools)


INSTALLATION
------------
Install as you would normally install a contributed drupal module.
See: https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------
If you want to unlink some default configuration, you should go to the admin
page of the module, and choose components which you want, and save the form.
Now, if you want to add this component to the feature, you should add this
component and variable "unlink_defaults" to the feature as well. So it is needed
for every version of the site (local, dev, stage, live) to have same
configuration for unlink default module.

To integrate this module with your own component you should do as follows:

 * to make sure you use ctools export. If not, you should implement
   hook_unlink_defaults_components (see unlink_defaults.api.php);

 * to make sure you use ctools export, If not, you should change order of
   implementation of hook default alter of your component
   (see example unlink_defaults_module_implements_alter());

 * if you use ctools export, in hook default alter of your component you should
   add such code like in unlink_defaults_views_default_views_alter(). If not,
   you should add your own logic to unlink configuration in hook default alter
   of your component like in
   unlink_defaults_default_rules_configuration_alter().


CHANGE STORAGE BACK TO FILES
----------------------------

First step should be to uncheck your configs from configuration page of
Unlink Default Config module. Then go to a list with configs (for example,
for views it is /admin/structure/views) and click "Revert" link near name of
your config. This process will remove config from database and automatically
switch config storage back to files.


API
---
See unlink_defaults.api.php


MAINTAINERS
-----------
Current maintainers:
 * Andrew Siz (AndrewsizZ) - https://www.drupal.org/u/andrewsizz
 * Evgeniy Maslovskiy (Spleshka) - https://www.drupal.org/u/spleshka
