SF-ACTIVE MIGRATE
-----------------

This project contains three modules:

 * sf-active migrate
   Uses the migrate framework to migrate content from an existing 
   sf-active database and filesystem.

 * sf-active features
   Sets up the content types and fields required to migrate sf-active 
   content.

 * sf-active legacy
   Various handlers for redirecting requests for old content.

Before installing please read INSTALL.txt.

Read more and git repositories:

 * https://drupal.org/project/sfactive

 * https://gitorious.org/sf-active/drupal


WHAT IS MIGRATED?
-----------------

Articles, comments, features, events, categories and users are migrated. 
This includes attached files on articles, comments and events and 
attached images on features. Attached text files are migrated to the 
database where they can be searched. User passwords still work 
(re-hashed with SHA-512 plus salt for better security). After migration, 
requests for legacy URLs are redirected to the new URL.


HOW TO MIGRATE
--------------

The migration can be run either from the UI provided by Migrate UI 
module or from the Drush command line. Before migrating, you must set 
some variables via Drush or the sf-active migrate settings page:

- The sf-active database name,
- The file directory path containing the uploads and images directories,
- The encoding applied to text files and blob database columns, and
- The time zone applied to all dates stored in the database.

Sample Drush command line:
  drush vset sfactive_migrate_database sfactive
  drush vset sfactive_migrate_files /var/www/sfactive/website 
  drush vset sfactive_migrate_encoding Windows-1252
  drush vset sfactive_migrate_timezone America/Los_Angeles
  drush mi --all
