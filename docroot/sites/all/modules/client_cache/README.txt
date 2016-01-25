Client Cache
============
Client cache is a Drupal caching backend that uses user agent or
client as cache bin.

Installation
============

 1. Install the client_cache module

 2. Edit settings.php to make one of the client cache
    plugin (cookie, HTML 5 storage etc..) as cache class for cache_client bin,
    for example:
      $conf['cache_backends'][] =
        'sites/all/modules/client_cache/client_cache.cookie.inc
      $conf['cache_class_client_cache'] = 'ClientCacheCookieDrupal';
