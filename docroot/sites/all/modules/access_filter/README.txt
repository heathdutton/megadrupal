
CONTENTS OF THIS FILE
---------------------

 * About Access Filter
 * Features
 * Configration
 * Todo

ABOUT ACCESS FILTER
--------------------

This module provides access control with paths/urls and IP addresses.


FEATURES
--------------------

Filter item:
  * Specify multiple paths/urls and IP addresses.
  * Path and url can use regex.
  * IP address can specify address range. (e.g. 192.168.0.1-192.168.0.10, 192.168.0.0/24)
  * Select action on denying access.
  * Test filter before save.

Fast mode:
  * This mode can avoid database access on loading filters.
    Please see /admin/config/people/access_filter to setup.


CONFIGRATION
--------------------

Getting started:
  * Open admin/config/people/access_filter to list filters.
  * Click "Add filter" to create new filter.

More settings:
  * Disabling module
    If you make a mistake setting filters, you can add below line to
    sites/default/settings.php to disable access control of this module.
    >--
    $conf['access_filter_disabled'] = TRUE; // Disable Access Filter access control.
    --<

TODO
--------------------

Add features:
  * More user friendly user interface.
