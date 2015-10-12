TABLE OF CONTENTS
-----------------

* Introduction
* Requirements
* Installation
* Configuration
* Troubleshooting
* FAQ
* Maintainers

INTRODUCTION
------------
This module provides an emergency "forced" delete option for 
[Drupal Commerce](https://www.drupal.org/project/commerce) orders. There are a
couple different ways that a order can get corrupted and then be undeleteable
by the normal delete functionality. This provides a 'Force Delete' command on
the order admin page (normally at: *admin/commerce/orders*) which will work
for any order even if it is very corrupted. However it is doing it through
forced SQL queries which means it isn't as 'Drupalish' or clean.

* To submit bug reports and feature suggestions, or to track changes, see the
[issue queue](http://drupal.org/project/issues/commerce_order_force_delete).

* See also the 
[module page](https://www.drupal.org/project/commerce_order_force_delete).

REQUIREMENTS
------------
This module requires [Drupal Commerce](https://www.drupal.org/project/commerce)
specifically with Commerce Orders enabled.

INSTALLATION
------------
Install as you would normally install a contributed Drupal module. See [install
instructions](http://drupal.org/documentation/install/modules-themes/modules-7)
in the Drupal documentation for further information.

CONFIGURATION
-------------
There are no configuration options:

TROUBLESHOOTING
---------------
* If the force delete link doesn't appear on the orders admin view, try the
following:

  * Clear your cache.

  * Try uninstalling and reinstalling the module.

FAQ
---
Any questions? Ask away on the issue queue or email: design@briarmoon.ca.

MAINTAINERS
-----------

Current maintainers:
* Nick Wilde (NickWilde) - https://www.drupal.org/u/nickwilde

This project has been sponsored by:
* BriarMoon Design
   Full service web development and design studio. Specializing in responsive,
   secure, optimized Drupal sites. BriarMoon Design can help you with all your
   Drupal needs including installation, module creation or debugging, themeing,
   customization, and hosting. Visit http://design.briarmoon.ca for more info.
