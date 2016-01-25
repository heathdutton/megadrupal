CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Recommended modules
 * Installation
 * Configuration

INTRODUCTION
------------

This module provides two new simple caches for views which takes view's
arguments into account:

* Custom cache: view's all arguments: All view's arguments, including
  contextual and exposed filters, will be taken into account.
* Custom cache: view's first argument: Only first view argument, even if it
  is a contextual or exposed filter, will be taken into account.

It is recommended to use this module in views which take arguments and where
cache hasn't to be content aware (doesn't matter if new content is added or
updated, cache will refresh when it expires according to user selection).
Performance gains will vary depending on views configuration and the amount
of results.

Module features:

* You can configure cache duration time
* You can enable/disable cache per role
* Works well with Views AJAX pagination

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/views_custom_cache

RECOMMENDED MODULES
-------------------

* Views argument cache. This module is very similar to Views custom cache
  except by the fact it doesn't allow to configure cache expiration time and
  doesn't allow to enable/disable cache per role. However, it allows you to
  choose how many arguments will have to be taken into account by cache. This
  module is great but I decided to do Views custom cache from scratch because
  it didn't work for me at all with views with exposed filters. And most
  important, this module requires you to manually flush caches because they
  don't automatically expire.

* Views content cache. This module is quite different of mine's because cache
  expiration is content based instead of time based like Views custom cache.
  You can set up this module to update cache once listed content is updated
  or a new related node has been created.

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

 * Edit a View you would like to cache. On "Advanced" column (third one) look
   for "Caching" option and click it. Then choose the cache type which better
   fits your view:

    - Custom cache: view's all arguments:

      Takes into account all views arguments.

    - Custom cache: view's first argument:

      Takes into account only first view argument.

* Then you will be prompted to choose cache options:

    - Cache duration time

      Choose expiration time for cache.

    - Custom time

      Cache duration time in seconds (this option is only available if "Custom"
      option is selected on previous "Cache duration time" select).

    - Cache per role?

      Enable or disable cache per each role.
