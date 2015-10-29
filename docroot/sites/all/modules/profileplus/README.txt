README.txt file for ProfilePlus module.

This module allows you to search all public fields in a user's profile. This includes the default
fields created by Drupal e.g. username, and any custom fields you might create with the Profile module.
Multiple keyword searching is also possible with this module. The returned results will link directly to
the user's profile.

INSTALLATION
============

1. Copy all files into their own folder, in the modules folder (preferably in sites/all/modules/profileplus).
2. Activate the module.
3. Check "Profileplus" under "Active search modules" at admin/config/search/settings.
3. Unlike 6.x version of profileplus, you do not need to override template variables to remove the tabs,
   this can be achieved by unchecking "Users" from "Active serach modules" at admin/config/search/settings.