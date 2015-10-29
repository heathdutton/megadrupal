<?php
// Original implementation of locating Drupal root from dursh module by Moshe Weitzman.
define('PHPFREECHAT_DRUPAL_BOOTSTRAP', 'includes/bootstrap.inc');

/**
 * Exhaustive depth-first search to try and locate the Drupal root directory.
 * This makes it possible to run drush from a subdirectory of the drupal root.
 */
function _phpfreechat_locate_root() {
  $path = getcwd();
  // Convert windows paths.
  $path = str_replace('\\', '/', $path);
  // Check the current path.
  if (file_exists($path . '/' . PHPFREECHAT_DRUPAL_BOOTSTRAP)) {
    return $path;
  }
  // Move up dir by dir and check each.
  while ($path = _phpfreechat_locate_root_moveup($path)) {
    if (file_exists($path . '/' . PHPFREECHAT_DRUPAL_BOOTSTRAP)) {
      return $path;
    }
  }
  return FALSE;
}

function _phpfreechat_locate_root_moveup($path) {
  if (empty($path)) {
    return FALSE;
  }
  $path = explode('/', $path);
  // Move one directory up.
  array_pop($path);
  return implode($path, '/');
}

/**
 * Bootstrap Drupal.
 */
// Change to Drupal root dir.
$base_path = _phpfreechat_locate_root();
if (!defined('DRUPAL_ROOT')) {
    define('DRUPAL_ROOT', $base_path);
}
chdir($base_path);

// Drupal will probably miscalculate the base_url if we're bootstrapping from handler.php
if (!isset($base_url)) {
  $base_root = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
  // As $_SERVER['HTTP_HOST'] allows user input, ensure it only contains valid characters.
  $base_url = $base_root . '://www.' . preg_replace('/[^a-z0-9-:._]/i', '', $_SERVER['HTTP_HOST']);
  // Unlike $_SERVER['PHP_SELF'], $_SERVER['SCRIPT_NAME'] cannot be modified by a visitor.
  $script_path_array = explode('/', $_SERVER['SCRIPT_NAME']);
  $base_path_array = explode('/', $base_path);
  $base_dir = implode('/', array_intersect($base_path_array, $script_path_array));
  $base_url .= $base_dir;
}

// Bootstrap Drupal.
require_once ('./includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
// Initialize configuration variables, using values from settings.php if available.
$conf = variable_initialize(isset($conf) ? $conf : array());
// Add file.inc for various file functions used in phpfreechat
require_once ('./includes/file.inc');
// Add common.inc to be able to use drupal_get_path later
require_once ('./includes/common.inc');
// Add user module to handle user permissions in phpfreechat
require_once (drupal_get_path('module', 'user') . '/user.module');

// We will need to load the current node
if (!function_exists('arg')) {
  // Use HTTP_REFERER to extract the node ID
  $path = str_replace(array($base_url . '/', '/?q='), '', $_SERVER['HTTP_REFERER']);
  // Get the real path from the current request
  require_once ('./includes/path.inc');
  $args = explode('/', drupal_get_normal_path($path));

  $result = db_query('SELECT * FROM {node} n INNER JOIN {phpfreechat} pfc ON pfc.nid = n.nid WHERE n.nid = :nid', array(':nid' => $args[1]));
  $node = $result->fetchObject();

  if ($_SESSION['drupal_user']) {
    $user = &$_SESSION['drupal_user'];
  }
}

// Starting phpfreechat module
require_once drupal_get_path('module', 'phpfreechat') . '/phpfreechat.module';
$params = phpfreechat_load_params(); // Load global params
$params = phpfreechat_prepare_params($params, $node, $user); // Prepare params for current node

// This is the phpFreeChat file with the code that gets everything going.
require_once drupal_get_path('module', 'phpfreechat') . '/phpfreechat/src/phpfreechat.class.php';
$chat = new phpFreeChat($params);

?>
