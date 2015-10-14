********************************************************************************
This module add a disclaimer to your website using Colorbox module with
configurable actions.

Visitors need javascript and cookie enabled.

********************************************************************************

Requirements
------------

This module require at least version 7.x of Drupal.
This module require Colorbox module 7.x-2.8+

Installation
------------

1. Copy the folder named 'disclaimer' and its contents to the modules directory
   of your Drupal installation (for example 'sites/all/modules/').

2. Go to 'admin/build/modules' and enable disclaimer.

3. Check your status to be sure Colorbox is properly installed
   (admin/reports/status).

4. Go to 'admin/config/system/disclaimer' and edit option and content for the
   disclaimer to your need.

Theming
-------

Use disclaimer.tpl.php file in your theme to override disclaimer structure.

Other settings
--------------

Disclaimer box designed is the colorbox one and is set from colorbox settings
page.
Age form format is set to the "Short" format of your date settings:
admin/config/regional/date-time

Test
----

Due to user access, disclaimer is never shown to administrator, logout to test
disclaimer.
You can see and delete cookie "disclaimerShow" once clicked. Use development
tools of your browser.

********************************************************************************

This module has been developed by mogtofu33 (Jean V)

Post a message on the drupal.org site if you have any ideas on
how we can improve the module.

Jean
contact@developpeur-drupal.com
