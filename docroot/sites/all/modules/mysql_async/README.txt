CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Usage
 * FAQ
 * Maintainers


INTRODUCTION
------------
The Asynchonous MySQL driver for Drupal, provides the option to perform asynchronous queries via DBTNG.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/mysql_async

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2422171


REQUIREMENTS
------------
 * Drupal 7
 * MySQL


INSTALLATION
------------
 * Copy (or symlink) the database subdir to the database driver location, so that Drupal can locate and use it.
 * %> cd /path/to/drupal/path/to/module
 * %> cp -r database /path/to/drupal/includes/database/mysql_async
 * Clear cache.

CONFIGURATION
-------------
 * Before enabling, remember to clear the cache.
 * To enable change the driver in settings.php

@code
  $databases['default']['default']['driver'] = 'mysql_async';
  $databases['default']['default']['async_max_connections'] = 10;
  $databases['default']['default']['async_min_connections'] = 2;

  // Use async queries inside transactions.
  $databases['default']['default']['async_transaction'] = TRUE;

  // Use async queries for all request. Experimental.
  $databases['default']['default']['default_options'] = array('async' => TRUE);

  // Use a connection timeout of 4 seconds. By default, the timeout specified
  // by $databases['default']['default']['pdo'][PDO::ATTR_TIMEOUT] will be
  // used.
  $databases['default']['default']['mysqli'] = array(PDO::ATTR_TIMEOUT => 4);
@endcode

Defaults for mysql_async:
@code
  array(
    'async_max_connections' => 30,
    'async_max_connections' => 5,
    'default_options' => array(
      'async' => FALSE,
      'async_transaction' => FALSE,
    ),
    'mysqli' => array(),
 );
@endcode

USAGE
-------------
Perform two heavy queries asynchronously:
@code
 // Initiate queries.
 $q1 = db_query("MY HEAVY QUERY ONE", array(), array('async' => TRUE));
 $q2 = db_query("MY HEAVY QUERY TWO", array(), array('async' => TRUE));

 // Wait no more than 30 seconds for queries to complete.
 $until = microtime(TRUE) + 30;
 do {
   $done = $q1->rowCount(TRUE) !== NULL && $q2->rowCount(TRUE) !== NULL;
 } while (!$done && microtime(TRUE) < $until);

 // Get results.
 var_dump($q1->fetchAll());
 var_dump($q2->fetchAll());
@endcode

FAQ
---
Q: Does my module need to require this module as a dependency?

A: No. Your code can be backwards compatible. By using rowCount(TRUE) to check if a request has finished, your code will still work when not using the mysql_async driver.

MAINTAINERS
-----------
Current maintainers:
 * Thomas S. Gielfeldt (gielfeldt) - https://drupal.org/user/366993

Inspired by APDQC - https://www.drupal.org/project/apdqc
