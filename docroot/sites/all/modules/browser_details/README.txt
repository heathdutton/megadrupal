Browser Details
===================

Introduction
--------------------------

Browser Details allows you to easily collect browser details on one page. This can be very useful for gathering information from clients in a technical support role.



Installation
----------------------------

1. Install PHP browscap.ini
     i) Download full_php_browscap.ini from http://tempdownloads.browserscap.com and place it somewhere in /etc/php5, e.g. /etc/php5/php_browscap.ini
    ii) Edit your php.ini file and look for [browscap], then add the following below it:
          browscap = /etc/php5/php_browscap.ini  # <-- this should be the FULL path to the location of php_browscap.ini
   iii)  Restart your web server, e.g. service apache2 restart


2. Download this module into sites/all/modules

3. Enable this module as you normally would in /admin/modules
     - if you'd like to see an example page with all the blocks on one page, enable the "Browser Details Panels" module, which depends on the Panels module.
       The example page can be viewed at /browser-details
     - otherwise, you can enable only the blocks that you're interested in, e.g. "Javascript Details".




Blocks
-----------------------------

Each type of detection, e.g. whether Javascript is enabled, can be added to any page using blocks.

Eg. Go to /admin/structure/block and look for "Browser Details: Javascript" if you enabled the "Javascript Details" module.



Author: Yonas Yanfa <http://drupal.org/user/473174>
