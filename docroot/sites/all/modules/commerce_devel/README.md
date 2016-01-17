Commerce Devel Module
=====================

This module is a starting place for Commerce Development support.

When a Drupal fatal error or exception occurs, it gets logged with a backtrace
to the screen if possible and to the watchdog log so it can be better
understood.

How to use it: Install it and enable it.

When something bad happens, look at admin/reports/dblog for more information.

There is now a "Commerce Devel Generate" module included that can generate
products and may in the future generate other entities. To use it, enable and
then navigate to admin/config/development/generate/product. There is also
a "drush genp" command to generate products.
