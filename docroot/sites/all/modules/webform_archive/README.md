Webform Archive
===============

Description
-----------
This module adds functionality of archiving webform submissions. It also allows
downloading reports for those archives. Such an archive will reduce overhead on
webform submission tables and the database queries for select and insert will be
faster thereby improving the performance.

Requirements
------------
Drupal 7.x


Dependencies
------------
Webforms


Installation
------------

1. Download the module and place in sites/all/modules directory OR use
drush dl webform_archive
2. Login as administrator. Enable the module in the "Administer" -> "Modules" OR
use drush en webform_archive
3. Goto admin/config/content/webform/archive and start archiving Submissions.

Functionality
-------------

1. Archive webform submissions based on time (for one or more webforms)
2. Generate reports for those archives

Drush Support
-------------

1. drush ard-webforms-details : Get the list of archived webforms
2. drush ar-active-webform : Archive active webforms submissions
3. drush ar-closed-webform : Archive closed webforms submissions
