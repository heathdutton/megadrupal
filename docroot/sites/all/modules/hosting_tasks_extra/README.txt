Hosting tasks extra
===================

This module extends Aegir's front-end with some additional tasks.

Supported tasks/commands are:

- Flush all caches (drush cache-clear -d)
- Rebuild registry (drush registry-rebuild -d)
- Run cron (drush core-cron -d)
- Run updates (drush updatedb -d)
- Flush the Drush cache (drush clear-cache drush -d)

Extra bundled modules:
- HTTP Basic Authentication (hosting_http_basic_auth)
- Sync (hosting_sync)

INSTALL
-------

This code is for hostmaster and needs to be uploaded in the modules directory
of the hostmaster platform.

It requires provision_tasks_extra extension uploaded in the ~/.drush/ directory
of your Aegir backend:

  http://drupal.org/project/provision_tasks_extra

In addition, for 'Rebuild registry', you will need to install an additional
Drush extension:

  http://drupal.org/project/registry_rebuild

