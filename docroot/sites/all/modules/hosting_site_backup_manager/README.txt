Hosting site Backup Manager
===========================

This module adds more backup options to Aegir.

- "Backups" tab to the "site" nodes in the Hostmaster environment.
  The tab shows the backups and enables per backup actions (Restore, Delete and Get).

- Specify how long Aegir should retain backups for.

- Specify during which time window backups should be made.

Installation
------------

1. Add as any other Drupal module into your hostmaster site.
2. Enable the feature in the 'experimental' section of the 'admin/hosting' page.

Configuration
-------------

1. Settings are available at 'admin/hosting/backup_manager' and allow you to e.g. choose
the numbers of backups to keep after specified periods of time.


TODO
----
- Check if there are tasks working with the backups to prevent "collisions"
