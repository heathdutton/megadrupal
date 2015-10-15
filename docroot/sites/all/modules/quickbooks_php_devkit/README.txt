
-- SUMMARY --

This module loads the QuickBooks PHP DevKit library into Drupal via the
Libraries API. The QuickBooks PHP DevKit is an open source QuickBooks PHP
library provided by ConsoliBYTE (http://consolibyte.com/).

For a full description of the module, visit the project page:
  http://drupal.org/project/quickbooks_php_devkit

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/quickbooks_php_devkit


-- REQUIREMENTS --

* Libraries API (2.x)
  https://drupal.org/project/libraries

* QuickBooks PHP DevKit library (>=2.x)
  http://consolibyte.com/downloads/quickbooks-php-devkit/


-- INSTALLATION --

* Download the QuickBooks PHP DevKit library from
  http://consolibyte.com/downloads/quickbooks-php-devkit/

* Extract the contents of the build/build_20130416 folder within the downloaded
  library into a quickbooks_php_devkit folder within a libraries directory
  (e.g., sites/all/libraries/quickbooks_php_devkit). A successful extraction
  will have a QuickBooks.php file located in the quickbooks_php_devkit folder
  (e.g., sites/all/libraries/quickbooks_php_devkit/QuickBooks.php).

* Install as usual, see
  http://drupal.org/documentation/install/modules-themes/modules-7

* Verify proper installation of the library at admin/reports/status. If the
  library is not properly installed, an installation error message will be
  present. If an unsupported version of the library is detected, a version error
  message will be present.


-- CONTACT --

Current maintainers:
* Jon Antoine (jantoine) - https://drupal.org/user/192192

This project has been sponsored by:
* Showers Pass
  Technically engineered cycling gear for racers, commuters, messengers and
  everyday cycling enthusiasts. Visit http://showerspass.com for more
  information.
