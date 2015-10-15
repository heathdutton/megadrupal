
typo3_migrate supports migrating content from a Typo3 website to Drupal
using the Migrate module. Import of the following is supported:
1. Users(both frontend and backend).
2. Standard typo3 pages along with their tt_content elements.
3. News(tt_news) and news categories.

The migrated content can be rolledback completely.

Dependencies:
-------------

The typo3_migrate module is based on the awesome Migrate module -  
http://drupal.org/project/migrate
typo3_migrate requires the Migrate module in version 7.x-2.1 or newer.

Note:
To import/rollback/etc. you can use drush commands or the UI provided
by the module migrate UI.

To better understand the migrate module:
see the migrate module documentation(http://drupal.org/node/415260)
and the economist.com case study(http://drupal.org/node/915102).

Usage:
------

IMPORTANT: Set the source typo3 database name at admin/settings/typo3
NOTE: the database must be accessible by the drupal db user and
must reside on the same db server.

FOR USERS:

1. Enable the typo3_users module.
2. For backend user migration run the command:
    drush migrate-import Typo3BeUser (See drush help for the complete listing of available Migrate commands)
   For rollback of the migration:
    drush migrate-rollback Typo3BeUser
3. For frontend user migration run the command:
    drush migrate-import Typo3FeUser
   For rollback of the migration: 
    drush migrate-rollback Typo3FeUser
    
Note:
    The username/password of the users will be retained in Drupal.
    If you want to ensure the length of the password check the patch 
    from http://drupal.org/node/1361150.
    Disabled users will not be migrated.
    
FOR TYPO3 PAGES:

1. Enable the typo3_pages module.
2. For Typo3 standard pages migration run the command:
    drush migrate-import Typo3Pages
   For rollback of the migration: 
    drush migrate-rollback Typo3Pages

Note:
    Only the standard typo3 pages will be migrated.
    Pages will be migrated to the "typo3_page" content type.
    Authoring information will be migrated from Typo3 to Drupal(Run the beuser migration first).
    tt_content of the type - 'text', 'textpic' and 'image' will be migrated.
    The <LINK> tag that some TYPO3 pages use will be replaced by a dummy <a> tag so that the drupal node body doesn't break.

FOR TYPO3 NEWS:

1. Enable the typo3_news module.
2. For News Category migration run the command:
    drush migrate-import Typo3NewsCategory
   For rollback of the migration: 
    drush migrate-rollback Typo3NewsCategory

Note:
    News Category will be migrated to the "News Category" vocabulary.
    news category hierarchy will be maintained.

3. For News migration run the command:
    drush migrate-import Typo3News
   For rollback of the migration: 
    drush migrate-rollback Typo3News
    
Note:
    By default news from all the folders is migrated. To limit the migration to a few folders pids can be entered at admin/content/typo3/news.
    Copy all the Typo3 news images to the pics folder inside the typo3_news module (OR replace the pics folder with a symlink, pointing to the original images folder).
    Copy all the Typo3 news files to the files folder inside the typo3_news module.
    News will be migrated to the "news" content type.
    Related news articles are handled through the "Related Articles" nodereference field.

Tips:
-----

1. It is recommended to keep the default input format of the Drupal site as full HTML.

The following TYPO3 tables are being used by the modules:
typo3_news module:

    tt_news_cat
    tt_news
    tt_news_cat_mm
    tt_news_cat
    tt_news_related_mm

typo3_pages module:

    pages
    tt_content

typo3_users module:

    fe_users
    be_users
