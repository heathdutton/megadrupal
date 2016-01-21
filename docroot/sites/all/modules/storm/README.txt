The Storm modules have been renamed to Project Management, and are now available from http://drupal.org/project/pm.

If you are wishing to migrate from an existing Storm installation (Drupal 6) to Project Management (Drupal 7):
1. Back up your website (files and database)
2. Upgrade core to Drupal 7.
3. Download Project Management, and place in your website's modules directory. DO NOT INSTALL OR ENABLE.
4. Remove all old Storm files.
5. Download the Storm Migrate module for Drupal 7 (this package).
6. Enable the Storm Migrate module.

This module will automatically migrate your content from Storm to Project Management, then enable the relevant Project Management modules and turn itself off. You may then remove the "Storm" directory from your website.

If you experience any problems, please report them at http://drupal.org/project/issues/storm.

