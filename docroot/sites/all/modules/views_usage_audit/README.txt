# Summary.

This module aims to help developers refactor a Drupal code base by
detecting unused view displays and providing information about the
menu paths that view displays are executed on.

Reports can be view at the following URL:

admin/reports/views-usage-audit-reports

Or you can get console output by running:

drush views-usage-audit-report-unused
drush views-usage-audit-report-usage

This module is designed to be highly performant and safe to enable
on production.

# Usage.

There are no configuration options for this module, simply enable it.

Views are logged as active on execution so you must exercise all parts
of your application in order to get completely accurate reports.

# TODO

* Logging of extra data: bundle, roles and maybe arguments.
* Tests.
