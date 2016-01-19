How-to

1 use node/$nid/salespush for Webform enabled content type's nodes for marking node as
to be pushed to SalesForce as campaign

2 Check SalespushWebformMapper::__construct for NID mappings and add if 0 does not fit Your needs.
Don`t forget - Email field is mandatory, all data will be skipped otherwise.

3 Enable salespush_ui and add all configurations at admin/config/services/salespush

4 Go to admin/config/content/salespush and use needed environment for working with.

5 Proper turn:
  1 push users,
  2 Users IDs sync,
  3 Push Campaigns,
  4 Push submissions.

6 You can enable cron at admin/config/services/salespush/environment "State of salespush cron"
and all 1-5 steps will be executed during cron task.


TODO

- Optimize cron runs and disable pushing users already stored in SalesForce
- Extend node/$nid/salespush form for ability to handle production/sandbox without admin form checking.
- Rework logs, returned from SalesForce for UI admin/config/content/salespush
- Do more deep checking of gathered data from webforms (watchdog screaming).
- Change SalespushWebformMapper from Singleton to non static class
- Rewrite hardcoded mappings in SalespushWebformMapper to setter() method
- Save all mappings in database and get them once SalespushWebformMapper initialized.
- refactor all modules to meet new structure (ex. salespush_webform_get_mappings())

