Description
-----------
This module enables MVC style of development within Drupal CMS

Requirements
------------
Drupal 7.x
Libraries API Module (http://drupal.org/project/libraries)

Installation
------------
1. Copy the entire prometheus directory to the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Site
   Building" -> "Modules"

3. For better AJAX support, install the History.js module:

  cd <site root>/sites/all/libraries
  git clone https://github.com/balupton/history.js.git

  Also ensure that Drupal "libraries" module has been downloaded and enabled

4. Make sure you always enable prometheus module first, before enabling other modules
that depend on it

Support
-------
Please use the issue queue for filing bugs with this module located at
https://github.com/drupalprometheus/prometheus/issues
