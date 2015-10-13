Boost Cache Cleaner Module
==========================

Dependencies - boost.

-----------------------------------------------------------------------------
There is no configuration required for this module because it is dependant on
boost module.
-----------------------------------------------------------------------------

About Boost Cache Cleaner
-------------------------
Boost cache cleaner flush (delete all the static the boost cache) when any 
update occurs in drupal core. Example node update, insert or delete.

Below are the events when the boost cache cleaner flush the cache.
------------------------------------------------------------------

1. Node insert, update and delete.
2. Blocks list admin display form update.
3. Individual block configure form.
4. New menu insert, update and delete.
5. Menu link insert, update and delete.
6. Taxonomy term insert, update and delete.
7. Taxonomy vocabulary insert, update and delete.
8. System theme setting form update.

-----------------------------------------------------------------------------
Module function is extendable in any custom module by calling 
boost_cache_cleaner_callback() function in your function.
-----------------------------------------------------------------------------
