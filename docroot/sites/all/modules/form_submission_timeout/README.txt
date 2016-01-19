MODULE
------
Form Submission Timeout


INTRODUCTION
------------

This module provides two main options (Countdown and Timed) followed by sub-options
(Frequency - Once, Everyday, Weedays and Weekends):

 * Countdown - You can limit form submission wihtin a certain time period after that
   form will automatically expire.

 * Timed - You can limit form submission on Frequency basis (Once, Everyday, Weedays and
   Weekends) e.g, if you want to limit your form submission for Once the form
   will only be available for provided date and time after that form will automatically
   expire. So as Everyday, Weedays and Weekends - form will be available for given
   date and time period on Everyday if selected Everyday, Weekdays if selected Weekdays
   and Weekends if selected Weekends.

    * For a full description of the module, visit the project page:
      https://drupal.org/project/form_submission_timeout


    * To submit bug reports and feature suggestions, or to track changes:
      https://drupal.org/project/issues/form_submission_timeout


REQUIREMENTS
------------
There are no dependencies for this module


Installation
------------

 * Copy the whole "form_submission_timeout" directory
   to your modules directory - "sites/all/modules/contrib/" or "sites/all/modules/"
 * Enable the module - "admin/modules"
   OR
 * Download the module via Drush - drush dl form_submission_timeout
 * Enable the module - drush en form_submission_timeout -y


CONFIGURATION
-------------

 * Go to "admin/config/development/form_submission_timeout" OR
   Configuration -> Development -> Form Submission timeout
 * Add entries for your forms.


MAINTAINERS
-----------
Current maintainers:
 * Gauravjeet Singh - https://www.drupal.org/u/gauravjeet
 * Zeeshan Khan - https://www.drupal.org/u/zeeshan_khan
 * Manjit Singh - https://www.drupal.org/u/manjit.singh
