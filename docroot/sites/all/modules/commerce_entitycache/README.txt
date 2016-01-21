Disabling Caching of Specific Entities
====================

If you want to disable caching of specific commerce entities, set the caching option in you settings.php to false

Example:

$conf['commerce_entitycache_cache_products'] = FALSE;
$conf['commerce_entitycache_cache_orders'] = FALSE;
$conf['commerce_entitycache_cache_line_items'] = FALSE;
$conf['commerce_entitycache_cache_customer_profiles'] = FALSE;

