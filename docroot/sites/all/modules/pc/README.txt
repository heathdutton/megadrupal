-= PHP Console Drupal module =-

Summary
=========================
This module provides integration with PHP Console server library.
PHP Console allows you to handle PHP errors & exceptions, dump variables,
execute PHP code remotely and many other things using Google Chrome extension PHP Console.
You can use pc() function which pretty prints any kinds of php variables.

Requirements
=========================
 * PHP 5.3 or higher
 * PHP Console extension must be installed on Google Chrome.
 * PHP Console library should be installed to sites/all/libraries/php-console directory.

Installation
=========================
 * Install as usual, see http://drupal.org/node/895232 for further information.
 * Download PHP Console library from https://github.com/barbushin/php-console.
 * Unzip the library to sites/all/libraries/php-console directory.
 * Navigate to PHP Console settings page: admin/config/development/php-console.
 * Set up your password and IP address in authorization section.
 * Check the module permissions: admin/people/permissions#module-pc.

Notes
=========================
 * You can use `drush pc-download` command for easy installation of the library.
 * For security reasons, it is recommended that you remove 'examples' and 'tests' directories from the library.
 * Some debugged variables may contain sensitive data. Do not allow untrusted roles view debug information.

Links
=========================
 * Project page: https://drupal.org/project/pc
 * PHP Console home page: http://php-console.com
 * PHP Console server library: https://github.com/barbushin/php-console
 * PHP Console Chrome extension library: https://chrome.google.com/webstore/detail/php-console/nfhmhhlpfleoednkpnnnkolmclajemef
 * PHP Console video presentation: http://www.youtube.com/watch?v=_4kG-Zrs2Io
