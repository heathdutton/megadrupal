## WHY YOU SHOULDN'T USE THIS MODULE ##

1. if your {variable} table is small.
2. if your site doesn't call variable_set() or variable_del() often.
3  if your site uses the db for cache. if this is true, stop installing this module
   and get that sorted.
4  if you're looking to speed up your site.
5. if you don't know how to patch core.
6. if you don't know what a cache backend is.

## WHY YOU SHOULD USE THIS MODULE ##

This module provides more predictable, less site-eating behaviour around
Drupal's variable system.

The caching around the {variable} table in core is braindead. For many sites, a
large variable table plus relatively regular variable_set() / variable_del()
(drupal_cron_run(), ooops) calls can cause a site outage.

This module reworks the caching layer so it can cope with a large number of
rows in the variable table, and makes the cache clearing that happens around
writes to the table, way, way, way less likely to take your site down.

This module requires a core patch, and can break your site and eat your lunch.

DON'T USE IT UNLESS YOU KNOW YOU NEED IT AND KNOW WHAT YOU'RE DOING.

## INSTALLATION ##

1. Apply the variables_that_suck_less.D7.core.patch from the root of your
   Drupal installation.
2. Enable the module.
3. Set $conf['variable_inc'] in settings.php:

// Change PATH_TO_MODULE_DIR to the installation directory of this module:
$conf['variable_inc'] = '{PATH_TO_MODULE_DIR}/variables_that_suck_less.variable.inc';

4. Optionally, if you are not using the databse as your cache backend, set
   $conf['cache_class_cache_variable'] in settings.php.

