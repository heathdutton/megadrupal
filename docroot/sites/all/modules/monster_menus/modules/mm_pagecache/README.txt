MM Page Cache
-------------

This module automatically clears Drupal page cache entries whenever a page or
node is updated, so that anonymous users don't see stale content. It can also
work with the Varnish module (http://www.drupal.org/project/varnish) to clear
Varnish's cache under the same circumstances.


Detailed Description
--------------------

When Drupal's page cache is enabled, anonymous users normally see a cached
version of a given page until it has expired from the cache, thus reducing CPU
load for pages that are viewed frequently.

This module enhances this feature by automatically purging the cache of certain
pages whenever any page or node on a page changes. Specifically:

- When a page's settings change, the caches for that page's parent and all of
  its children are purged.
- When a node changes, the caches for any pages on which the node resides are
  purged.

Furthermore, if the Varnish module is used, the caches Varnish maintains are
also purged.


Configuration
-------------

Both the automatic cache clearing and Varnish cache clearing can be disabled by
visiting admin/config/development/performance.

If you do use the varnish module, you should disable its internal cache clearing
by either setting "Varnish Cache Clearing" to "None" on
admin/config/development/varnish or by removing this line from your
settings.php:

  $conf['cache_class_cache_page'] = 'VarnishCache';

If you do not disable the varnish module's internal cache clearing in this way,
all Varnish caches will be cleared every time a node is saved, which defeats
the purpose of mm_pagecache.


Caveats
-------

This module does not delete cache entries for most pages or nodes that are shown
as blocks within other pages. Addressing this shortcoming would simply be too
prohibitive to calculate. Therefore, if your site uses blocks that change
frequently, it is still recommended that a relatively short value be used for
"Expiration of cached pages" in admin/config/development/performance.