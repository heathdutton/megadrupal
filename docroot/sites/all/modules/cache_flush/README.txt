This project allows to flush cache for drupal blocks and entities
(nodes, users, taxonomy terms). It has dependency on contextual links.
For blocks now supported standard drupal blocks, views and panels.
After installation you will be able to:

1) Flush entities cache (use "Flush Cache" local tasks on entities pages);
2) Flush blocks, views cache using "Flush Cache" contextual link.
3) Flush panels cache using "Flush Cache" prefix link.
4) You can flush any cache bin using admin page
yoursite.com/admin/config/development/cache-flush

To remove all cache bin you need to select '*' in Cache id input, select cache
table and check wildcard. Users with advanced skills can set any cache id and
clear cache as it allows cache_clear_all function
https://api.drupal.org/api/drupal/includes%21cache.inc/function/cache_clear_all/7.

Implemented acquia_purge module integration.
