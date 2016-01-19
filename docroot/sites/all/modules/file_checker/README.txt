File Checker README
-------------------

In a perfect Drupal world your server filesystem and its correspoding entries in Drupal's files table are 100% synchronized.
But what if parts of your file system have been corrupted due to some disk failure? Or one of your modules messed up your
database and files? Or your deploy script went beserk? Well, then this module will help you to monitor and find out which files
are out of sync.

Out of the box the files table has two kind of statuses: Temporary (0) and Permanent (1). The file checker introduces an additional
status Missing (2). In scope of the verification process which can be triggered in various ways the status column of the files
table is updated.

Features
--------

- Run verification process
  - on demand
  - via cron
  - via drush (in planning)
- File list overview page with filters
- Views integration

If you want to export the results of a view it's recommended to use views_data_export module.

Usage
-----
The file checker can be configured on admin/settings/file_checker. Take a look at the "Process batch size" first. Ideally you shouldn't run
too many iterations, e. g. 100.000 files / 10.00 batch size = 10 runs sounds like a good approach. By pressing the button "Flag missing files"
you can run the file verification manually.

On admin/reports/file_checker you can view the results.