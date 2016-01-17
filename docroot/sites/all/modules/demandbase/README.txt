The module provides integration with demandbase api.
Visit http://wiki.demandbaselabs.com/mwiki/index.php?title=Demandbase_Drupal_Connector for more details.

Installation:
--------------
Copy the entire "demandbase" folder into [ROOT]/drupal/sites/all/modules

Note: you may need to install the CTools and/or Context modules first.


Usage:
------

IMPORTANT: Set the demandbase key at admin/config/services/demandbase

Using Context
- Create context using the demandbase context conditions.

Using API
- You can use the following snippet of code to get the visitor's information:

  // Get an object of demandbase data for the visiting user.
  $data = demandbase_get_data();

DEMANDBASE CACHE
================

Demandbase provides two types of caching modules
1) demandbase_cache
2) demandbasee_memcache

1) demandbase_cache override drupal default caching for anonymous user and store page cache in cache_page table for all pages or for specified paths if specified under admin/config/services/demandbase/cache path.

2) demandbase_memcache override drupal default caching for anonymous user and store page cache under memcached service which store cache on RAM for all pages or for specified paths if specified under admin/config/services/demandbase/cache path.

NOTE: demandbase_memcache will take priority over demandbase_cache module if following line is added in settings.php file :
$conf['cache_class_cache_page'] = 'DemandBaseMemCacheDrupal';// this will take preference above demandbase_cache

To enable demandbase_memcache add these following lines to settings.php file:

$conf['cache_backends'][] = 'sites/all/modules/demandbase/demandbase_memcache/demandbase_memcache.inc';
$conf['cache_default_class'] = 'DemandBaseMemCacheDrupal';
$conf['cache_class_cache_page'] = 'DemandBaseMemCacheDrupal';  // this will take preference above demandbase_cache
$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
//$conf['session_inc'] =  'sites/all/modules/demandbase/demandbase_memcache/demandbase_memcache-session.inc';

$conf['memcache_servers'] =  array('localhost:11211' => 'default');