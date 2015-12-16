
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * FAQ
 * Maintainers

INTRODUCTION
------------
The Views in CiviCRM Dashlets module allows creation of customizable dashlets
with integrated Views displays.

  * For a full description of the module, visit the project page:
    - https://www.drupal.org/sandbox/brandonferrell/2389543

  * To submit bug reports and feature suggestions, or to track changes:
    - https://www.drupal.org/project/issues/2389543?categories=1

REQUIREMENTS
------------
This module requires the following modules:
 * CiviCRM (https://www.drupal.org/project/civicrm)
   - This must be version 4.4+
 * Views (https://www.drupal.org/project/views)

INSTALLATION
------------
Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

CONFIGURATION
-------------
1. Visit your View and add a new CiviCRM Dashlet display
2. Customize your display at will and save (Make sure to set all of the Dashlet
   settings!)
3. Visit www.yourdomain.com/civicrm and click "Configure Your Dashboard"
4. Your new dashlet will be in the Available Dashlets bank. Drag your dashlet
   into either the left or right column and click Done
5. Your dashlet will now be on your dashboard!

TROUBLESHOOTING
---------------
* If your dashlet is not saving correctly, check the following:
  - Is the weight you set used by another dashlet? If so, change one of them so
    that they are not matching.

FAQ
---
* Q: I edited my view, but my dashlet hasn't updated. Why isn't my new view
     being rendered?
  A: Click "Refresh Dashboard Data" on the dashboard, just to the right of
     "Configure Your Dashboard".

MAINTAINERS
-----------
Current maintainers:
 * Brandon Ferrell (brandonbferrell) https://drupal.org/user/2930039
 * Mark Hanna (markusa) https://www.drupal.org/user/142798

This project has been sponsored by:
 * Skvare
   Skvare builds online presence and community management tools for non-profit
   organizations, professional societies, membership-driven associations, and
   small businesses. Visit https://skvare.com for more information.
