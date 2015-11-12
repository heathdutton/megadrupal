User Instructions
=================
Date: 2009-09-05
  Subject: successfully tested with apache-php-5.3.0
  Subject: successfully tested with postgresql-8.4.0
  Subject: successfully tested with mysql-5.1.37

Developer Push Down List
========================
Date: 2009-08-22
  Subject: name space cleanup in watchdog() first arg is also limited to 16 characters
    As soon as the lenght limit disappears, change the identifiers:
    - Search Files Dir
    - Search Files Att (unused)
    + Search Files Directories
    + Search Files Attachments (unused)

Date: 2009-08-15
  Subject: search_files_directories.module needs to handle overlapping directory trees when indexing
  Subject: search_files_directories.module needs to handle duplicate directory entries on Add/Edit

Date: 2009-08-14
  Subject: Titles and Descriptions should no longer be wrapped in t(), see http://drupal.org/node/140311
  Subject: node MIME type vs. search_files EXTENSION file classification

Date: 2009-07-26
  Subject: name space cleanup
    This module is almost name space clean, including function names, database
    tables and columns or identifiers in foreign database tables. However, the
    "search" module put a dirty little length limit in the `type` column of the
    {search_dataset} table. Same for {search_index} table. As soon as the
    lenght limit disappears, change the identifiers:
    - search_files_dir
    - search_files_att
    + search_files_directories
    + search_files_attachments

