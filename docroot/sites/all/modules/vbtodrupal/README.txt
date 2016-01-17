
CONTENTS OF THIS FILE
---------------------

  - Introduction
  - Installation
  - Configuration and migration
  - Known issues


INTRODUCTION
------------

Current Maintainer: Liam McDermott.
Contact via: http://www.intermedia-online.com/#contact

vBulletin to Drupal imports vBulletin into Drupal forums. There are two modules
included: vBulletin to Drupal and vBulletin Passwords. The former copies data
from vBulletin, the latter allows users to login using vBulletin usernames
and passwords.

Note, imports have sucessfully been run for:

  - vBulletin 3.6.2
  - vBulletin 3.8.0
  - vBulletin 4.0.5


INSTALLATION
------------

vBulletin to Drupal is installed in the same way as most other Drupal modules.
Here are the instructions for anyone unfamiliar with the process:

1. Copy this vbtodrupal/ directory to your sites/SITENAME/modules directory.

2. Back in your Web browser, navigate to Administer -> Modules
   then enable the vBulletin to Drupal module.


CONFIGURATION AND MIGRATION
---------------------------

The configuration page for vBulletin to Drupal can be found by browsing to
Administer -> Structure -> vBulletin to Drupal.

1. Configure your vBulletin database settings. It's important that the user name
   used has read access to the vBulletin database and _write access_ to Drupal
   or the import will fail.

   Also, notice the Database type field is greyed out in the configuration tab,
   due to a limitation in Drupal's database API, the vBulletin database must be
   of the same type as Drupal's.

2. Click the 'Test and import from vBulletin' tab, and carefully examine the
   results. This will examine the Drupal environment, and give instructions on
   any action needed to ensure a successful import. Refresh the page after
   making changes to check again.

3. If the 'Test and import from vBulletin' page finds no problems, it is safe
   to click 'Import from vBulletin'. This will *immediately* start the import
   process.

4. The migration process will take several minutes. For a large forum, expect
   at least 15 minutes for an import to run, allow 30 minutes before worrying
   that the process has stalled.

5. When the process is complete you will be taken back to the Configure page.
   Any errors that occurred during the import process will be displayed, if
   they are not obviously related to your setup open an issue from the
   vBulletin to Drupal project page (http://drupal.org/project/vbtodrupal).

Good luck with your migration, and if you need paid support, see the contact
information at: http://www.intermedia-online.com

Important note for Organic Groups users: do not use the stable 7.x-1.4 version!
Please use 7.x-2.0-alpha3 or later.


KNOWN ISSUES
------------

1. Missing posts/threads: if there are missing forum posts and threads after,
   running an import, first check whether there were any errors shown by
   vBulletin to Drupal. If not, the most likely cause of missing posts is
   duplicate users: users being imported from vBulletin that are already in the
   Drupal database. When vBulletin to Drupal attempts to import these user
   accounts into the Drupal {users} table, the import silently (that is, no
   error messages are shown) ignores these accounts.

   As these accounts are not imported, the forum threads/posts belonging to
   these accounts will not appear in the Drupal forums.
 
   This problem has been seen when converting a Drupal vB
   (http://drupal.org/project/drupalvb) installation, but should be checked for
   on any vBulletin to Drupal conversion -- particularly when the Drupal
   installation is not a newly installed one.

   This has been fixed in a recent update, please re-open the following issue
   if you see this problem on your site: http://drupal.org/node/281861

2. Invalid argument supplied for foreach() ... taxonomy.module on line 1214:
   This occurs whenever a cron job runs, to view updates to this issue see:
   http://drupal.org/node/477176

   There is currently no solution to this, yet. It also occurs in other data
   import modules.

3. Rebuilding comment threads can take a very long time. On a site where the
   forum posts are flat and not threaded, it may be desirable to skip this
   step. To do so add: $conf['vbtodrupal_skip_comment_threads_rebuild'] = TRUE;
   to your site's settings.php
