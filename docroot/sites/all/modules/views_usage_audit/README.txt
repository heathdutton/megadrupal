INTRODUCTION
------------

This module aims to help developers refactor a Drupal code base by
detecting unused view displays and providing information about the
menu paths that view displays are executed on.

Reports can be view at the following URL:

admin/reports/views-usage-audit

Or you can get console output by running:

drush views-usage-audit-report-unused
drush views-usage-audit-report-usage

This module is designed to be highly performant and safe to enable
on production.


REQUIREMENTS
------------

This module requires the following module:
* Views (https://drupal.org/project/views).


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. See: 
https://drupal.org/documentation/install/modules-themes/modules-7 
for further information.

There are no configuration options for this module, simply enable it.

Views are logged as active on execution so you must exercise all parts
of your application in order to get completely accurate reports.


TODO
----

* Logging of extra data: bundle, roles and maybe arguments.
* Tests.


MAINTAINERS
-----------

Current maintainers:
 * Alan MacKenzie (alanmackenzie) - https://www.drupal.org/u/alanmackenzie

Development of this module was part funded by:
 * BBC Worldwide
   Successfully scaled to handle the traffic of bbcgoodfood.com.
   Visit www.bbcworldwide.com for more information.
