-------------------------------------------------------------------------------
Multisite Wizard for Drupal 7.x
  by Alex Posidelov, Archer Software
-------------------------------------------------------------------------------
DESCRIPTION:

This module simplifies the process of converting single site to multisite, 
make sure that the site administrator did all folder-files 
steps (see http://groups.drupal.org/node/121989) and ending of auto copying 
and creating tables with new prefixes in same database. So you don't need 
to use any phpmyadmin or sql works... just click on a button!

FEATURES:

* Tables with new prefix are createn to current database;
* Module cannot drop and rename the tables of current site;
* Module copy and add prefix to ALL tables of current site (you cannot share some tables between multisite through admin panel - TODO);
* Module doesn't allow to create 2 settings.php files with same prefixes;

INSTALLATION:

* Put the Backup and Migrate module in your drupal modules directory.
* Put the module in your drupal modules directory and enable it in 
  admin/modules. 
* Go to admin/people/permissions and grant permission to any roles that need to be 
  able to use Process page.
* Use the module at admin/config/multisite_wizard

-------------------------------------------------------------------------------