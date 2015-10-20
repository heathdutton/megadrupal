
Extra Site Information
----------------------


CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Usage


INTRODUCTION
------------

Extra Site Information module provides Drush commands and UI level view to see
Count of Content types, Nodes, Users, Roles existing in the site.
This module also provides the information about Currently loggedin users
in your site.


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


USAGE
-----

 * "nodetypecount" command will return the number of Content Types present
   in the site, you can use command in following ways.
   drush nodetypecount --help
   drush nodetypecount

 * "nodecount" command will return the number of Nodes present in the site,
   you can use command in following ways.
   drush nodecount --help
   drush nodecount
   drush nodecount < content-type-names >
   drush nodecount page article

 * "rolecount" command will return the number of Roles present in the site,
   you can use command in following ways.
   drush rolecount --help
   drush rolecount

 * "usercount" command will return the number of Users existing in the site,
   you can use command in following ways.
   drush usercount --help
   drush usercount
   drush usercount < role-ids >
   drush usercount 3 4

 * Visit "admin/reports/extra-siteinfo" for UI level view of data.

 * For Currently loggedin users in your site.
   Visit "admin/reports/extra-siteinfo/currently-loggedin-users".
   You can end the session of any user by clicking "end session" button.
