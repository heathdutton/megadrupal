CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Tests
 * FAQ
 * Maintainers

INTRODUCTION
------------
The Cache Consistent module provides a transactional aware cache backend wrapper,
ensuring that cache is synchronized with database transactions.

This module is an attempt to solve the root cause of https://www.drupal.org/node/1679344.

Cache Consistent works best with the database isolation level is set to READ-COMMITTED.
It CAN be used with REPEATABLE-READ, but will in this case only mitigate the problem,
not eliminate it all together. When configured for REPEATABLE-READ, there may also
be occasionally more cache-misses.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/cache_consistent

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/cache_consistent


REQUIREMENTS
------------
 * Drupal 7


INSTALLATION
------------
 * Download and enable the module (https://www.drupal.org/documentation/install/modules-themes/modules-7)
 * Apply the core patch included with the module.
 * Clear cache.


CONFIGURATION
-------------
A transactional aware cache wrapper, that buffers cache operations until transaction has been committed.

Example configuration:
@code

// Include apropriate backends.
$conf['cache_backends'] = array(
  'sites/all/modules/contrib/memcache/memcache.inc',
  'sites/all/modules/contrib/cache_consistent/cache_consistent.inc',
);

// Set default cache backend to use Cache Consistent.
$conf['cache_default_class'] = 'ConsistentCache';

// Use regular database backend for form cache (because it's not a cache).
$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

// Set Cache Consistent's default backend to memcache.
$conf['consistent_cache_default_class'] = 'MemCacheDrupal';

// Don't skip cache_set() operations during transactions.
// When using the isolation level REPEATABLE-READ, safe mode should be set to
// TRUE. When using READ-COMMITTED, it should be FALSE.
// What safe mode does, is that it discards cache writes during transactions, to
// avoid storing stale data into the cache. This is due to transaction snapshots
// and MVCC.
$conf['consistent_cache_default_safe'] = FALSE;

// Set cache consistent's buffer mechanism to ConsistentCacheBuffer. (default)
// There's currently only one buffer mechanism bundled with Cache Consistent.
$conf['consistent_cache_buffer_mechanism'] = 'ConsistentCacheBuffer';

// Don't do strict expire checking on each item for all bins. (default)
$conf['consistent_cache_default_strict'] = FALSE;

// Strict expire checking on each item for cache_bootstrap.
$conf['consistent_cache_strict_cache_bootstrap'] = TRUE;

@end


TESTS
-----
The tests bundled with the module requires PHP 5.4 due to the use of traits.


FAQ
---
Q: Why do I need this?

A: Whenever cache operations occur inside a transaction, the result of the cache
operation will be visible to concurrent requests, even though the transaction
hasn't been committed yet (or even rolled back). See https://www.drupal.org/node/1679344.

Q: But my site is running fine??? ... I think?

A: A lot of sites may run fine without using Cache Consistent. If you're using only
the database as a cache backend, you don't even need Cache Consistent. But the minute
you use another backend for cache, you introduce the possibiliy of a race condition
that can result in the cache being populated with old data. One situation where
this especially can be seen is when using entitycache. This makes a non-database
cache backend very susceptible to a race condition that can result in nodes being
uneditable due to the "this form has changed" error, which can then only be resolved
by clearing cache.

Q: But if I just clear the cache, the site works again and runs fine?

A: Yes. That is correct. And if you like that solution, just stick to it.


MAINTAINERS
-----------
Current maintainers:
 * Thomas S. Gielfeldt (gielfeldt) - https://drupal.org/user/366993
