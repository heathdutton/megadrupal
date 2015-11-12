To use graceful cache for entire system use the following in your settings file:

$conf['cache_backends'] = array(
  'sites/all/modules/cache_graceful/cache_graceful.inc',
);

$conf['cache_default_class'] = 'GracefulCache';
$conf['graceful_cache_default_class'] = 'DrupalDatabaseCache';

