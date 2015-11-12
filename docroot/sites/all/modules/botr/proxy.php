<?php
/**
 * @file
 * A JSON interface to the part of the Bits on the Run API that is used
 * by the widget.
 */

require_once 'botr_api.php';

$_BOTR_PROXY_METHODS = array(
  '/videos/list',
  '/channels/list',
  '/videos/create',
  '/videos/thumbnails/show',
);

/**
 * Bootstrap Drupal, in order to use its user system (rights) and read
 * the settings (API key and secret).
 */
function botr_bootstrap() {
  $boot_file = './includes/bootstrap.inc';

  /*
   * We need to bootstrap Drupal, but don't know how deep
   * in the directory tree we are. Therefore, we first try
   * look for a 'sites' directory in our path. Its parent
   * should be the Drupal basedir.
   */

  $module_dir = $_SERVER['SCRIPT_FILENAME'];
  $dirs = explode(DIRECTORY_SEPARATOR, $module_dir);

  // Search for the 'sites' directory.
  $keys = array_keys($dirs, 'sites');
  // Take the innermost 'sites' directory.
  $index = $keys[count($keys) - 1];

  if ($index !== FALSE) {
    $dirs = array_slice($dirs, 0, $index);
    $basedir = implode(DIRECTORY_SEPARATOR, $dirs);
    chdir($basedir);

    if (file_exists($boot_file)) {
      define('DRUPAL_ROOT', $basedir);
      require_once $boot_file;
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      return TRUE;
    }
  }

  /*
   * Previous method has failed. Now, try looking for the bootstrap file in each
   * of our parent directories.
   */

  chdir($module_dir);
  $prev_dir = NULL;

  while (getcwd() != $prev_dir) {
    if (file_exists($boot_file)) {
      define('DRUPAL_ROOT', getcwd());
      require_once $boot_file;
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      return TRUE;
    }
    else {
      $prev_dir = getcwd();
      chdir('..');
    }
  }

  return FALSE;
}

/**
 * Output a JSON-style error message.
 */
function botr_json_error($message) {
  $message = json_encode($message);
  return '{ "status" : "error", "message" : ' . $message . '}';
}

/**
 * Act as a proxy between the widget and the API kit.
 */
function botr_proxy() {
  global $_BOTR_PROXY_METHODS;

  if (!user_access('use botr')) {
    echo botr_json_error(t('Access denied'));
    return;
  }

  // Read the API method.
  $method = $_GET['method'];

  if ($method === NULL) {
    echo botr_json_error(t('Method was not specified'));
    return;
  }

  if (!in_array($method, $_BOTR_PROXY_METHODS)) {
    echo botr_json_error(t('Access denied'));
    return;
  }

  // Get the key and secret from the Drupal settings.
  $api_key = variable_get('botr_api_key', '');
  $api_secret = variable_get('botr_api_secret', '');

  if (!$api_key || !$api_secret) {
    echo botr_json_error(t('Enter your API key and secret first'));
    return;
  }

  $botr_api = new BotrAPI($api_key, $api_secret);
  $params = array();

  foreach ($_GET as $name => $value) {
    if ($name != 'method' && $name != 'q') {
      $params[$name] = $value;
    }
  }

  $params['api_format'] = 'php';
  $response = $botr_api->call($method, $params);
  echo json_encode($response);
}

if ($_REQUEST['method'] == 'upload_ready') {
  // This supplies a valid target for the redirect after the upload call.
  echo '{ "status" : "ok" }';
}
else {
  if (botr_bootstrap()) {
    botr_proxy();
  }
  else {
    die('Could not bootstrap Drupal install.');
  }
}
