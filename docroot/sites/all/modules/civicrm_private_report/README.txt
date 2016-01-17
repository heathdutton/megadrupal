OVERVIEW
--------

This module allows users to create their own private copies of CiviCRM reports,
which they can modify and save to meet their needs, without relying on an
administrative user to create the report, and without having access to
administer reports themselves. This way users can easily have the data they
need, without affecting reports that are relied upon by other users.


KEY FEATURES
------------

* Each user has a private space for reports visible and editable by that user
  alone.
* Users can copy any report that they can access into their own private report
  space.
* CiviCRM's native listings of available reports do not show user-private
  reports, but do provide a link to the user's own list of private reports.
* Any user-private report is accessible only by the user who created that report,
  with the exception of administrative users.
* Administrative users can view any user-private report and may promote it to
  site-wide availability (that is, removing the private ownership status of
  the report).


USEFUL MENU LINKS
------------------------------

This module provides the following pages:

* http://example.com/civicrm_private_report/list
  Lists all existing user-private reports belonging to the current user.
* http://example.com/civicrm_private_report/list/all
  Lists all existing user-private reports for all users, grouped by user. This
  page is accessible only to administrative users.


PERMISSIONS
---------------------

Users must have the following permissions to make use of this module's features.

* Creation and management of one's own user-private reports:
  CiviCRM module:
    - access CiviCRM
    - access CiviReport
    - access Report Criteria
  CiviCRM Private Report module:
    - administer own CiviCRM reports

* Reviewing user-private reports and promoting them to site-wide availability:
  All of the above, plus:
  CiviCRM module:
    - administer reports


HELP AND IMPROVEMENTS
---------------------

Please help us improve this module by using the module issue queue to report
any troubles and to make requests for feature improvements. The module queue is
here:
http://drupal.org/project/issues/civicrm_private_report?status=All&categories=All
