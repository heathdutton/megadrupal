Panels Hash Cache
-----------------
This module provides a cache plugin for Panels, Panelizer and other
CTools-based display systems. The primary goal of the module is to cache panes
and pages indefinitely until certain events occur, at which point that pane is
reloaded.

This version of the module only works with Panels v7.x-3.4 and newer due to the
use of the cache table added in 3.4.


Features
------------------------------------------------------------------------------
The primary features include:

* Support for all Page Manager (CTools), Panels and Panelizer displays.

* Provides a large list of cache granularity options:

  * The current page's arguments.

  * The current Panels display's contexts.

  * The full URL, including query strings.

  * The current menu path and callback arguments.

  * The active user.
    * This will create a separate cache object for each user and is usually
      unnecessary.

  * The active user's roles.
    * This allows for creating a separate cache instance based upon the current
      user's roles. This is particularly useful when using the Contextual
      module, to avoid the context menus showing for the wrong users.
    * It is possible to select which roles will be given the same cached object
      as the anonymous user; it is suggested to select the "Authenticated user"
      role on scenarios when a "normal" logged in user would not gain any extra
      access to the content.
    * In the interest of performance, it is possible to trigger caching based
      on a complete list of all a given user's roles, just the first one or
      just the last one; the default is to consider all of a user's roles.

  * The current domain, as configured through Domain Access.


Related modules
------------------------------------------------------------------------------
Some available modules are similar or related to Panels Content Cache:

* Cache Consistency
  https://www.drupal.org/project/cache_consistent
  Sites using something other than the database for cache storage, e.g.
  memcached, may run into a bug in Drupal core that triggers a race condition
  in the cache_field bin. This module provides a work-around and is in use
  on drupal.org. See https://www.drupal.org/node/1679344 for further details.

* Panels Content Cache
  https://www.drupal.org/project/panels_content_cache


Credits / Contact
------------------------------------------------------------------------------
Originally written and developed by Andrey Tretyakov [1], Patrick Hayes [2] and
Dipen Chaudhary [2], additional maintenance by Ajit Shinde [3] and Damien
McKenna [4].

The best way to contact the maintainers is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/panels_hash_cache


References
------------------------------------------------------------------------------
1: https://www.drupal.org/u/crea
2: https://www.drupal.org/u/phayes
3: https://www.drupal.org/u/dipen-chaudhary
4: https://www.drupal.org/u/ajits
5: https://www.drupal.org/u/DamienMcKenna
