CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Other
 * FAQ
 * Maintainers


INTRODUCTION
------------
The Cache Heuristic module gathers information on which cache entries are used
on a page, and bulk-loads these cache entries on following requests.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/cache_heuristic

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/cache_heuristic


REQUIREMENTS
------------
 * Drupal 7


INSTALLATION
------------
 * Enable the module as usual.

CONFIGURATION
-------------
 * To enable heuristic caching change the cache backend in settings.php


@code
  // Load the cache heuristic backend.
  $conf['cache_backends'] = array(
    'sites/all/modules/contrib/cache_heuristic/cache_heuristic.inc'
  );

  // Use cache heuristic as default cache backend
  $conf['cache_default_class'] = 'HeuristicCache';

  // Use the deferred database cache backend for HeuristicCache
  // (optional, the DrupalDatabaseCacheDeferred is used by default if not specified)
  $conf['heuristic_cache_default_class'] = 'DrupalDatabaseCacheDeferred';

  // Use another database connection for the field cache.
  $conf['cache_target_cache_field'] = 'my_form_connection';

  // Use tombstone flushing for all bins.
  $conf['cache_default_tombstone_flush'] = TRUE;

  // Use database cache directly for cache form (i.e. don't use cache heuristic for cache form).
  $conf['cache_class_form'] = 'DrupalDatabaseCache';

  // Use mem cache as default backend for cache heuristic (requires the memcache module)
  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['heuristic_cache_default_class'] = 'MemCacheDrupalDeferred';
@endcode


STATIC CACHING
--------------
Static caching is provided through the 'static' option, either globally or per bin.
@code
  // Enable static caching for all bins.
  $conf['heuristic_cache_default_static'] = TRUE;

  // Disable static caching for a single bin.
  $conf['heuristic_cache_static_cache_bootstrap'] = FALSE;
@end


BUFFER SIZE
-----------
The number of cache entries collected by Cache Heuristic can be controlled through
the option 'buffer_size', either globally or per bin.
Static caching is provided through the 'static' option, either globally or per bin.
@code
  // Infinite buffer size for all cache bins. (default)
  $conf['heuristic_cache_default_buffer_size'] = -1;

  // Only collect 10 cache entries for cache_field.
  $conf['heuristic_cache_buffer_size_cache_field'] = 10;
@end


THRESHOLD
---------
To avoid writing collected cache entries to the database all the time, a threshold
can be set.
@code
  // If any bin has changed more than 5 entries in its buffer, then store the
  // collection. (default)
  $conf['heuristic_cache_default_threshold'] = 5;

  // If there's any difference, whatsoever, in the cache_bootstrap collection,
  // the store the collection.
  $conf['heuristic_cache_threshold_cache_bootstrap'] = 0;
@end

@code
  // If there are more than 20 changes across all bins, store the collection.
  // (Disabled by default)
  $conf['heuristic_cache_totalthreshold'] = 20;
@end


OTHER
-----
Cache Heuristic also provides a standard database cache backend similar to the
one from Drupal core, but with the following extra features:

 * Configurable database target
 * Tombstone flush mechanism

@code
  // Load the cache heuristic backend.
  $conf['cache_backends'] = array(
    'sites/all/modules/contrib/cache_heuristic/cache_heuristic.inc'
  );

  // Use cache heuristic as default cache backend
  $conf['cache_default_class'] = 'DrupalDatabaseCacheTarget';

  // Use another database connection for the form cache.
  $conf['cache_target_cache_form'] = 'my_form_connection';

  // Use tombstone flushing for all bins.
  $conf['cache_default_tombstone_flush'] = TRUE;
@endcode


FAQ
---
Q: Does it work as excepted.

A: I hope so.

MAINTAINERS
-----------
Current maintainers:
 * Thomas S. Gielfeldt (gielfeldt) - https://drupal.org/user/366993

