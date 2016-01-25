Overview
--------
No Devel gracefully handles PHP errors that are caused by calls to dpm() and
related functions of the Devel module when Devel is not installed/enabled.
Anyone who has encountered "Call to undefined function dpm()" during
development, or accidentally "white screened" an entire live site when moving
code containing a stray Devel function call to staging and production servers
may find use for No Devel.

No Devel is intended to be installed as insurance on live servers and to aid
development, particularly in multiple-server environments with multiple
developers, where installed modules and configurations may vary (not as an
excuse to sloppily leave unnecessary function calls in production code).


Features
--------
* Devel function calls (when Devel is not enabled) are handled as warning
  messages for administrators and/or warnings in Drupal's log, instead of raw
  PHP errors.
* The following Devel functions are protected against: dpm(), dvm(), dpr(),
  dvr(), kpr(), dargs(), dd(), ddebug_backtrace(), and db_queryd(). For more
  information on these functions see: http://drupal.org/project/devel


Requirements
------------
* Drupal 7 (though a backport to Drupal 6 is intended)


Installation
------------
Install as usual, and enable.
  http://drupal.org/documentation/install/modules-themes/modules-7
  
  
Configuration
-------------
Visit the configuration page at /admin/config/development/nodevel where you may
configure whether No Devel displays a warning message to administrators and/or
logs errors in Drupal's log for Devel module function calls when Devel is not
enabled.
  
  
Contact
-------
Chris Komlenic - drupal.org/user/1411194/
