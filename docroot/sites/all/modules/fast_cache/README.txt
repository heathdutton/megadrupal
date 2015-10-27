============================
======= Installation =======
============================

1. Install the module as usual.

2. Add the following configs to your settings.php or settings.local.php file:
   $conf['cache_backends'][] = 'sites/all/modules/fast_cache/backends/fast_cache.inc';
   $conf['cache_default_class'] = 'FastCache';

3. If you're using the database cache backend (default) - then this is it!

4. If you're using a custom cache backend (memcached, redis, etc) then add the
   following configs to your settings.php:

   # If you're using the same cache backend for all cache bins:
   $conf['fast_cache_default_class'] = 'MemcacheStorage'; // Or any other cache class name.

   # If you want to use different cache backends for different cache bins:
   $conf['fast_cache_default_class'] = 'MemcacheStorage';
   $conf['fast_cache_class_cache_form'] = 'DrupalDatabaseCache';
   $conf['fast_cache_class_cache_update'] = 'DrupalDatabaseCache';

============================
======= Advanced ===========
============================

At the moment the module has one experimental feature: lazy cache. This feature
collects a list of cache set/delete requests during the request and commits them
together at the end of request.

Benefit of this feature:
* Really good performance when your site has a lot of set/delete cache requests.
  For example, during cache clear operation the difference is significant.

Negative side of this feature:
* When running several concurrent requests when cache instances are empty, then
  each request will have to collect data for cache and will set it to the cache
  backend at the end of request.

To enable this feature add the following line to settings.php file:

$conf['fast_cache_lazy_cache'] = TRUE;

Lazy load has one more cool feature - possibility to set all cache data as one
request to the cache backend. If you cache backend class implements "setMultiple"
method, then it will be used to set all caches.

If you're using database as a cache backend together with lazy cache feature of
this module then you should definitely add the following to settings.php:

$conf['cache_backends'][] = 'sites/all/modules/fast_cache/backends/database.inc';
$conf['fast_cache_default_class'] = 'FastCacheDatabase';

It just adds implementation of "setMultiple" method for DB cache and improves
performance for cache set requests.
