<?php
$xhprof_enabled = FALSE;

if (function_exists('xhprof_enable') && $xhprof_enabled) {
  include_once '/Users/marco/PhpstormProjects/xhprof' . '/xhprof_lib/utils/xhprof_lib.php';
  include_once '/Users/marco/PhpstormProjects/xhprof' . '/xhprof_lib/utils/xhprof_runs.php';
  xhprof_enable(XHPROF_FLAGS_CPU + XHPROF_FLAGS_MEMORY);
}

// @TODO: this does not support installations in a subdirectory, and has been
// tested with Apache only.
$is_https = isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) == 'on';
$http_protocol = $is_https ? 'https' : 'http';
// Set the global $base_url. We need to set it if we're using ad_serve.php in the
// module directory, or Drupal will set a wrong one and images and links will be
// wrong.
$base_url = $http_protocol . '://' . $_SERVER['HTTP_HOST'];
// Set the Drupal root.
define('DRUPAL_ROOT', substr($_SERVER['SCRIPT_FILENAME'], 0, strpos($_SERVER['SCRIPT_FILENAME'], $_SERVER['REQUEST_URI'])));
// There's code, for example in phptemplate.engine, which assumes we're in the
// Drupal root directory.
chdir(DRUPAL_ROOT);

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';
// Perform a minimal bootstrap.
drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);
drupal_add_http_header('X-Ad-bootstrap-phase', 'configuration');

// We'll try to load advertisement from the cache.
require_once DRUPAL_ROOT . '/includes/cache.inc';
foreach (variable_get('cache_backends', array()) as $include) {
  require_once DRUPAL_ROOT . '/' . $include;
}

// Decide whether to bootstrap the database.
$cache_backend = _cache_get_object('ad');
if ($cache_backend instanceof DrupalDatabaseCache) {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
  drupal_add_http_header('X-Ad-bootstrap-phase', 'database');
}

// Lock.inc could be used by cache backend (e.g. memcache stampede protection).
require_once DRUPAL_ROOT . '/' . variable_get('lock_inc', 'includes/lock.inc');

// We use the queue API to store the impression.
require_once DRUPAL_ROOT . '/modules/system/system.queue.inc';
// In my tests, including system.queue.inc directly is a lot faster than
// module_load_include(); this also makes sure that module_implements() does not
// exist, and therefore if watchdog() (or any other function that calls
// module_implements()) is called, the cache of module_implements() is not
// saved.
//require_once DRUPAL_ROOT . '/includes/module.inc';
//module_load_include('inc', 'system', 'system.queue');
// This looks like the cache_backends setting, but it's not in core.
foreach (variable_get('queue_backends', array()) as $include) {
  require_once DRUPAL_ROOT . '/' . $include;
}

if (variable_get('ad_queue_impressions')) {
  $queue = DrupalQueue::get('ad');
  // Using the DB queue backend may still make sense, for sites with a very
  // long full bootstrap.
  if ($queue instanceof SystemQueue) {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
    drupal_add_http_header('X-Ad-bootstrap-phase', 'database');
  }
}

require_once DRUPAL_ROOT . '/' . variable_get('ad_module_path') . '/ad.module';

ad_get_ads();

if (function_exists('xhprof_enable') && $xhprof_enabled) {
  $namespace = 'Site-Install';
  $xhprof_data = xhprof_disable();
  $xhprof_runs = new XHProfRuns_Default();
  $devel_run_id = $xhprof_runs->save_run($xhprof_data, $namespace);
  drupal_add_http_header('X-Ad-xhprof', $devel_run_id);
}
