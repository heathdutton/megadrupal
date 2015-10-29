OVERVIEW
Multisite Search allows you to index and search content from all sites in a Multisite configuration.
If you are looking for more powerful search integration, try the Apache Solr module:
http://drupal.org/project/apachesolr

Thanks to lebachai for permission to share with the community.
This module was initially developed for Cleveland Public Library, a website based on Drupal.


REQUIREMENTS
This modules assumes the following is true:
- You have a Multisite Drupal installation
- You are sharing tables
- all of your databases are on the same host and use the same username and password


MULTISITE CONCEPTS
Drupal can be configured to run multiple websites from a single code base.
To read more about multisite configuration, read the resources here: http://drupal.org/node/43816

Drupal database tables in a Multisite installation can be in separate databases, or in a single 
database using table prefixing.


RECOMMENDED CONFIGURATION
For the easiest Multisite configuration, it is recommended that you include a separate database
for _all_ of your shared tables.

For example, if you site1, site2, site3, and you're sharing tables, the databases would look like this:

  site1
  site2
  site3
  shared - contains all your shared tables

If separate tables are not possible, this can also be done with table prefixing.

For example:

  site1_
  site2_
  site3_
  shared_ - all your shared tables


SHARING MULTISITE SEARCH TABLES
It is a good idea to share the Multisite Search database tables. These include the search index
and module configuration. This way, you don't have to add configuration settings multiple times.

More information: http://drupal.org/node/22267 (slightly out of date)

Edit each settings.php file and make the following edits:

Your $databases array has an extra variable called 'prefix'. If your database looks like this:

$databases = array (
  'default' => 
  array (
    'default' => 
    array (
      'database' => 'drupal_local',
      'username' => 'username',
      'password' => 'password',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => 'local_',
    ),
  ),
);

Change it to this:

$databases = array (
  'default' => 
  array (
    'default' => 
    array (
      'database' => 'drupal_local',
      'username' => 'username',
      'password' => 'username',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => array(
        'default' => 'local_',
        'multisite_search_dataset'  => 'shared_db.',
      	'multisite_search_index'    => 'shared_db.',
      	'multisite_search_sites'    => 'shared_db.',
      	'multisite_search_total'    => 'shared_db.',
      	'multisite_search_settings' => 'shared_db.',
      ),
    ),
  ),
);

Where 'shared_db' is the name of your shared database. You can also use table prefixing if you only have one database,
in which case you would use _ (underscore) instead of . (period).

For more information, see code comments of settings.php and http://drupal.org/node/22267 .


INSTALLATION INSTRUCTIONS

See INSTALL.txt


TABLE PREFIXING
The follow are some examples of how to fill out the "Database Name" and "Table Prefix" field.
Database name is required even if they're all using the same database.

Example 1: One database, table prefixing

Site 1 - database.site1_
Database Name: database   Table Prefix: site1_

Site 2 - database.site2_
Database Name: database   Table Prefix: site2_

Site 3 - database.site3_
Database Name: database   Table Prefix: site3_


Example 2: separate databases, no prefixing

Site 1 - db1.
Database Name: db1   Table Prefix: (blank)

Site 2 - db2.
Database Name: db2   Table Prefix: (blank)

Site 3 - db3.
Database Name: db3   Table Prefix: (blank)


Example 3: mixed

Site 1 - db1.
Database Name: db1   Table Prefix: (blank)

Site 2 - db2.site2_
Database Name: db2   Table Prefix: site2_

Site 3 - db3.site3_
Database Name: db3   Table Prefix: site3_


CONFIGURATION SETTINGS
Once the sites are set up, visit admin/settings/multisite-search/settings to change settings.
Note: These settings only need to be changed once.

Refresh Multisite search index (in seconds):

This controls how frequently the search index is rebuilt. The default is 0 seconds, which means that the 
entire Multisite Search index will be rebuilt on every cron run (for every site). On sites with a lot of 
content, it is recommended to set this to perhaps daily (86400) or weekly (604800).

Search block label:

This is the text that is displayed on the Multisite Search block.

Search tab label:

This is the text that is displayed on the search results page tab.

Exclude unpublished nodes:

Default is to exclude. Note that core search ignores node access and only checks on node display.
This has been fixed with some SQL trickery: http://drupal.org/node/1190056
If, for some reason, you do want to search unpublished nodes, uncheck this box.

Exclude content types:

This is similar to the unpublished issue. Since Multisite Search doesn't have access to the node permissions 
in the other sites, it doesn't know what to skip. More SQL trickery has been added to work around this.


MULTISITE SEARCH BLOCK
Multisite Search block has been removed in Drupal 7 version. This is because the core search block can be
configured to point to multisite search.

To set up core search block to use Multisite search:

1. Visit admin/config/search/settings.
2. Under "Default search module", select "Multisite Search".
3. Visit admin/structure/block and add the "Search form" block to your favorite region.
