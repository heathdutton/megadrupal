README_memcache_profiler.txt
============================

memcache_profiler.inc provides a simple cache_backend class which extends the MemCacheDrupal class.

##### NOTE: It is not necessary to enable the memory_profiler module to use memcache_profiler.inc #####

The purpose of this is to log the size of objects which Drupal sends to memcache when it does a cache_set.

The main reason you might want to measure the size of what Drupal is sending to memcache is to identify the source of errors like this:

"failed with: SERVER_ERROR object too large for cache"

By default memcache has a 1m limit on the values it can store for each key, but the PECL memcache extension compresses data it sends to memcache by default.

Therefore this profiling class records both the uncompressed size of a cache value, and the size in bytes of the entry when compressed in (what should be) the same way the PECL memcache extension compresses values it's sending to memcache.

This is an example of how to configure the memcache_profiler class in conjunction with the memcache cache backend in your settings.php

$conf['cache_backends'][] = './sites/all/modules/memcache/memcache.inc';
$conf['cache_backends'][] = './sites/all/modules/memory_profiler/memcache_profiler.inc';
$conf['cache_default_class'] = 'ProfileMemCacheDrupal';
$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

Like the main memory_profiler module, this should be lightweight enough to run in production without a serious performance impact so long as syslog rather than dblog is in use.

There are some variables within the class which should allow you to turn the logging on and off easily, and control the thresholds for logging. The defaults are based on a limit of 1m.

Example output using syslog:

Jul 15 12:04:35 server-123 mysite: http://example.com|1405425875|profile_memcache|123.45.67.89|http://example.com/node/123||0||bin: cache_field | key: field:node:123 | len: 4658423 | comp: 3526750
Jul 15 12:04:36 server-123 mysite: http://example.com|1405425876|php|123.45.67.89|http://example.com||0||Notice: MemcachePool::set() [memcachepool.set]: Server server-123.hosting..com (tcp 11211, udp 0) failed with: SERVER_ERROR object too large for cache

The first entry is from profile_memcache, and the second is the actual error when memcache tries to store the object which is too large.

The information which profile_memcache logs is:

  bin: the cache bin which is the destination of the cache_set
  key: the key (or cid) passed to cache_set
  len: length in bytes of the (serialized) data to be cached
  comp: length in bytes of the data to be cached after compression

A useful one-liner to analyse the output:

grep '|profile_memcache|' drupal-watchdog.log | grep -o '|bin.*| len' | sed 's/ | len$//g;s/^|//g' | sort | uniq -c | sort -nr
  462 bin: cache_field | key: field:node:320396
  315 bin: cache_field | key: field:node:305061
  311 bin: cache_field | key: field:node:286871

This shows counts of (predicted) errors along with the bin and key for each.

