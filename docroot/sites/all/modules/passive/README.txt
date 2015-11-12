Passive cache
=============

This module allows passive refresh of non-permanent cached entries. For example, the passively cached result of a page
is refreshed transparently, returning the stale result to the user while a sub-request is triggered to update the cache.

Authenticated page cache is available via the Authcache module (using the built-in backend).

Some configuration requirements and tips for maintaining performance:

* The default Drupal cron must not be enabled (or a page request may be liable for multiple backend sub-requests).
* Memcache as the passive cache backend is recommended.
* This module only behaves differently for temporary cache (and will thus have no effect on regular application cache).
  Examples of temporary cache include pages (i.e. 'cache_page') and blocks.

To use passive cache as the backend for any cache bin, the passive.cache.inc file must be specified as a 'cache_backend'
entry in settings.php before any page cache backend (e.g. Authcache). This is to allow Passive to indicate to other
backends not to attempt to retrieve page cache for a (verified) backend refresh in order for cache to be rebuilt. For
example, if Passive is installed under 'sites/all/modules/contrib/passive':

  $conf['cache_backends'][] = 'sites/all/modules/contrib/passive/passive.cache.inc';


Page cache
----------
Configure settings.php to use PassiveCache for 'cache_page'.

The passive backend configuration is similar to Drupal cache backend configuration, but with the setting keys prefixed
with 'passive_'. For example, $conf['passive_cache_default_class'] specifies the backend for passive cache. If not set,
the Drupal default backend is used, i.e. $conf['cache_default_class'].

Example Drupal configuration with Memcache:

  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_backends'][] = 'sites/all/modules/contrib/passive/passive.cache.inc';
  $conf['lock_inc'] = 'sites/all/modules/contrib/memcache/memcache-lock.inc';
  $conf['memcache_stampede_protection'] = TRUE;
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
  $conf['cache_class_cache_page'] = 'PassiveCache';
  $conf['cache_class_cache_block'] = 'PassiveCache';
  $conf['passive_cache_class_cache_block'] = 'DrupalDatabaseCache';

By default, a backend refresh is unique across page URL and session ID for the sake of basic function. For security
reasons, it is HIGHLY RECOMMENDED that the $conf['passive_request_index'] setting be configured to the relevant use
case, i.e. 'passive_request_anon_index' for Drupal-only anonymous page cache or 'passive_request_authcache_index' for
Authcache key variants.


Passive Authcache reverse proxy
-------------------------------
A passive reverse proxy is responsible performing sub-requests as directed by the backend response. The purpose of this
sub-request is to make the individual Authcache pages cacheable by the reverse proxy and minimize traffic between proxy
and backend to effectively just a session cookie lookup (and passive page cache refreshes).

The reverse proxy must:

* Perform a sub-request on 'HTTP/1.1 363 Passive Prompt' (custom) from backend using the X-Passive-Request and
  X-Passive-Key headers. (The response code can be overridden in case 363 is already used for something else.)
* Indicate it supports passive requests via X-Passive-Proxy: true.

If the proxy is not configured as passive, performance can still be optimized by passive page cache (only the traffic
will not be minimized).

The following is a sample Varnish VCL for the passive proxy, e.g. as included from the main VCL:

  sub vcl_recv {
    // Mark as passive proxy.
    set req.http.X-Passive-Proxy = "true";

    if (req.restarts < 1 && req.http.X-Passive-Key) {
      remove req.http.X-Passive-Key;
    }
  }

  sub vcl_deliver {
    // Restart passive prompt.
    if (resp.status == 363) {
      if (resp.http.X-Passive-Request) {
        set req.http.X-Passive-Request = resp.http.X-Passive-Request;
      }
      if (resp.http.X-Passive-Key) {
        set req.http.X-Passive-Key = resp.http.X-Passive-Key;
      }
      return (restart);
    }

    // Clean up response header.
    if (resp.http.Vary ~ "(?i)X-Passive-Key") {
      // Remove Vary: X-Passive-Key to replace with Vary: Cookie.
      set resp.http.Vary = regsuball(resp.http.Vary, "(?i)(^|,\s*)X-Passive-Key,\s*", "\1");
      if (!resp.http.Vary ~ "(?i)Cookie") {
        set resp.http.Vary = "Cookie, " + resp.http.Vary;
      }
    }
  }

To configure Drupal to serve a passive prompt to a passive proxy for Authcache, the 'passive_proxy/authcache.inc' must
be specified as a cache backend AFTER 'passive.cache.inc' and 'authcache.cache.inc' and BEFORE
'authcache_builtin.cache.inc'. For example:

  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_backends'][] = 'sites/all/modules/contrib/passive/passive.cache.inc';
  $conf['cache_backends'][] = 'sites/all/modules/contrib/authcache/authcache.cache.inc';
  $conf['cache_backends'][] = 'sites/all/modules/contrib/passive/passive_proxy/authcache.inc';
  $conf['cache_backends'][] = 'sites/all/modules/contrib/authcache/modules/authcache_builtin/authcache_builtin.cache.inc';
  $conf['passive_request_index'] = 'passive_request_authcache_index';
  $conf['lock_inc'] = 'sites/all/modules/contrib/memcache/memcache-lock.inc';
  $conf['memcache_stampede_protection'] = TRUE;
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
  $conf['cache_class_cache_page'] = 'PassiveCache';


Overriding default behavior
---------------------------
Various functions in this module can be overridden to use custom behavior. A number of variables define the function to
use for the action.
