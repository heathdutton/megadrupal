<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 - User Data Connector module - Standalone DRUPAL server script.
 * -----------------------------------------------------------------------------
 *
 * It must be invoked by a http/https POST request from the same server.
 *
 * Returns a json encoded string containing the first line:
 * { key:value, key:value, processtime:"<double>" }
 *
 * The second line can be "ERR" to signal an unexpected error. If so additional
 * information will be provided in the following lines.
 *
 * This script will bootstrap Drupal minimally and use the user validation
 * functions to check the incoming request.
 *
 * @author Stefan Wilhelm (stfwi)
 * @package org.drupal.project.udc
 * @license GPL
 */


// Initialisation of variables before Drupal is called and can modify POST
$_process_time = microtime(TRUE);
$_request = $_POST; // + $_GET -- not recommended to use get for this
$_debug = array();

/////////////////////////////////////////////////////////////////////////////////
//// Required functions
/////////////////////////////////////////////////////////////////////////////////


/**
 * Used to exit immediately without informing the client about the reason
 */
function kill_connection($text='') {
  header('Unauthorized', TRUE, 401);
  if (empty($text)) {
    die("<h1>Not Authorized</h1>");
  }
  else {
    die("$text");
  }
}


/**
 * Attaches debugging information.
 */
function add_debug($text, $serv = "notice") {
  global $_debug;
  $_debug[] = "$serv: $text\n\n";
}


/**
 * Error reporting overload
 * @param int $errno
 * @param string $errstr
 * @param string $errfile
 * @param string $errline
 */
function checkuser_error_handler($errno, $errstr, $errfile, $errline) {
  switch ($errno) {
    case E_NOTICE:
    case E_STRICT:
    case E_USER_DEPRECATED:
    case E_USER_NOTICE:
    case E_COMPILE_WARNING:
    case E_CORE_WARNING:
    case E_USER_WARNING:
      add_debug("$errstr ($errfile:$errline)");
      return TRUE;
  }
  die(response(NULL, "$errstr ($errfile:$errline)"));
}


/**
 * Exception overload
 * @param Exception $e
 */
function checkuser_exception_handler($e) {
  die(response(NULL, $e->getMessage()));
}


/**
 * Send the results
 * @param mixed $data
 * @param mixed $error
 */
function response($data, $error = FALSE) {
  global $_process_time;
  global $_debug;
  $out = array(
    'status' => empty($error),
    'data' => array(),
    'processtime' => round(microtime(TRUE) - $_process_time, 3)
  );
  if (!empty($data))  $out['data'] = $data;
  if (!empty($error)) $out['error'] = $error;
  if (!empty($_debug) && UserDataConnectorConfig::instance()->getDebugEnabled()) $out['debug'] = $_debug;
  $out = json_encode($out);
  // session_destroy(); not initialized
  while (ob_get_level() > 0) ob_get_clean();
  print $out;
  return '';
}

/////////////////////////////////////////////////////////////////////////////////
//// Initial host check, no proceeed in Drula context or foreign host
/////////////////////////////////////////////////////////////////////////////////

if (defined('DRUPAL_ROOT')) {
  throw new Exception("User Data Connector Server cannot be included in Drupal context.");
}
elseif ($_SERVER['REMOTE_ADDR'] != $_SERVER['SERVER_ADDR']) {
  kill_connection();
}
elseif (stripos($_SERVER['HTTP_USER_AGENT'], 'dp-user-connector-client') === FALSE) {
  kill_connection('Your software is not not authorized');
}

/////////////////////////////////////////////////////////////////////////////////
//// Init Drupal
/////////////////////////////////////////////////////////////////////////////////

ob_start();

// Override DRUPAL_ROOT, the server assumes the root to be the document root
for($path=dirname(__FILE__); !is_file("$path/index.php"); $path=dirname("$path")) {
  if (stripos($path, $_SERVER['DOCUMENT_ROOT']) === FALSE) {
    throw new Exception('Module path is not in the server document root');
  }
}
define('DRUPAL_ROOT', $path);
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');

// Override function return of conf_path() defined in bootstrap.inc. This function
// normally autodetects which site is addressed, hence which settings.php shall
// be included.
if(!empty($_request['site'])) {
  $conf = 'sites/' . $_request['site'];
  if (!is_file(DRUPAL_ROOT . '/' . $conf . '/settings.php')) {
    throw new Exception("User Data Connector Server: No such site ('$conf')");
  }
  else {
    drupal_static('conf_path', $conf);
  }
}

// Deal with the Simple Test tables if the client base class sent a post key
// called 'simpletest-db-prefix', but double check if this data is plausible.

if (isset($_request['simpletest-db-prefix'])) {

  // Check if really a simpletest prefix - It is very improbable that the
  // simpletest prefix occurrs in any normal database.
  if (strpos($_request['simpletest-db-prefix'], 'simpletest') === FALSE) {
    throw new Exception('The Simple-Test database prefix does not contain the word "simpletest", so not allowed');
  }

  // Load/include config
  drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);

  // Check if the database config exists
  if (!isset($databases) || !isset($databases['default']) || !isset($databases['default']['default'])) {
    throw new Exception('Configuration $databases[\'default\'][\'default\'] missing');
  }

  $databases['default']['default']['prefix'] = $_request['simpletest-db-prefix'];
  drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
  header('Content-Type: text/plain');
  set_error_handler('checkuser_error_handler', E_ALL | E_STRICT);
  set_exception_handler('checkuser_exception_handler');

  // Check if the user table exists with the modified database prefix e.g. prefix
  // is dp_simpletest0123456, then the user table is dp_simpletest0123456user
  foreach (db_query("SHOW TABLES")->fetchCol() as $v) {
    if (strpos($v, $_request['simpletest-db-prefix'] . 'user') !== FALSE) {
      $_request['simpletest-db-prefix'] = TRUE;
      break;
    }
  }

  if ($_request['simpletest-db-prefix'] !== TRUE) {
    throw new Exception('User table not found using the Simple-Test database prefix');
  }
}

// Perform minimal bootstrap for a functioning server ecript but least possible
// overhead
drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);

// Override the Drupal error and exception handlers, set content type
header('Content-Type: text/plain');
set_error_handler('checkuser_error_handler', E_ALL | E_STRICT);
set_exception_handler('checkuser_exception_handler');

// Include server script dependencies
require_once(dirname(__FILE__) . '/../include/UserDataConnectorConfig.class.inc');
require_once(dirname(__FILE__) . '/../include/UserDataConnectorDatabase.class.inc');

/////////////////////////////////////////////////////////////////////////////////
/// Process request
/////////////////////////////////////////////////////////////////////////////////

if (!isset($_request['request'])) {
  throw new Exception('No request');
}
elseif ($_request['request'] == 'ping') {
  // Ping must work without token
  response(array('time' => time()));
  exit();
}
elseif (UserDataConnectorConfig::instance()->getToken() == '') {
  throw new Exception('Server config not set');
}
elseif (UserDataConnectorConfig::instance()->getRequireHttps() && ($_SERVER['SERVER_PORT'] != 443)) {
  throw new Exception('Server wants HTTPS');
}
elseif (!isset($_request['token'])) {
  kill_connection();
}
else {
  $token = UserDataConnectorConfig::instance()->getToken();
  if (strpos($token, 'file:') === 0) {
    $token = trim(str_replace('file:', '', $token));
    if (!empty($token) && $token[0] != '/') {
      $token = $_SERVER['DOCUMENT_ROOT'] . '/' . $token;
    }
    if (($token=file_get_contents($token)) === FALSE) {
      throw new Exception('Token file not existing or not readable');
    }
    else {
      $token = trim($token, " \n\r\t\v");
    }
  }
  if ($_request['token'] != $token) {
    kill_connection();
  }
  unset($token);
}


if ($_request['request'] == 'auth' || $_request['request'] == 'info') {

  ////////////////////////////////////////////////////////////////////////////
  /// Request = auth || info
  ////////////////////////////////////////////////////////////////////////////

  if ($_request['request'] == 'auth') {
    require_once(DRUPAL_ROOT . '/includes/password.inc');
  }

  $data = $_request['data'];

  $cudb = new UserDataConnectorDatabase();
  $cudb->loadAvailableFields(isset($data['fields']) ? $data['fields'] : FALSE);

  // Process database request
  if ($_request['request'] == 'auth' && empty($data['pass'])) {
    throw new Exception('No password given');
  }
  elseif (empty($data['mail']) && empty($data['name'])) {
    throw new Exception('Neither user name nor mail given');
  }

  $query = db_select('users', 'u')->fields('u', array('uid', 'name', 'pass', 'mail', 'status'))->condition('uid', 0, '>');
  if (!empty($data['active'])) $query->condition('status', 0, '>');
  if (!empty($data['name'])) $query->condition ('name', $data['name'], '=');
  if (!empty($data['mail'])) $query->condition ('mail', $data['mail'], '=');
  if (!empty($cudb->allowedRoleIds)) $query->where("(SELECT COUNT(*) FROM {users_roles} AS r WHERE u.uid=r.uid AND r.rid IN(:rids))", array(':rids' => $cudb->allowedRoleIds));
  if (!empty($cudb->unlistedUsers)) $query->condition ('name', $cudb->unlistedUsers, 'NOT IN');
  $account = $query->execute()->fetchAllAssoc('uid');

  // Check account
  if (count($account) > 1) {
    throw new Exception('Redundant user names');
  }
  elseif (count($account) < 1)  {
    throw new Exception('User not found');
  }
  else {
    $account = reset($account);
    if ($account->uid < 1 || ($_request['request'] == 'auth' && !user_check_password($data['pass'], $account)) ) { // This takes a bit time!
      throw new Exception('User not found');
    }
    else {
      // Format output
      $account->active = $account->status > 0;
      $account->roles = $cudb->getUserRoles($account->uid);
      $account->fields = $cudb->getUserFields($account->uid);
      unset($account->pass);
      unset($account->status);
      unset($account->uid);
      response($account);
      exit();
    }
  }
}
elseif ($_request['request'] == 'list') {

  ////////////////////////////////////////////////////////////////////////////
  /// Request = list
  ////////////////////////////////////////////////////////////////////////////

  $cudb = new UserDataConnectorDatabase();

  $data = $_request['data'];
  $with_roles = isset($data['withroles']) && (bool) $data['withroles'];

  $query = db_select('users', 'u')->fields('u', array('uid', 'name', 'mail', 'status'))->condition('u.uid', 0, '>');
  if (!empty($cudb->allowedRoleIds)) $query->where("(SELECT COUNT(*) FROM {users_roles} AS r WHERE u.uid=r.uid AND r.rid IN(:rids))", array(':rids' => $cudb->allowedRoleIds));
  if (!empty($cudb->unlistedUsers)) $query->condition ('name', $cudb->unlistedUsers, 'NOT IN');
  if (!empty($data['active'])) $query->condition('status', 0, '>');
  $query = $query->execute()->fetchAllAssoc('uid');

  $response = array();
  foreach ($query as $user) {
    $res = array(
      'name' => $user->name,
      'mail' => $user->mail,
      'active' => $user->status > 0,
    );
    if ($with_roles) $res['roles'] = $cudb->getUserRoles($user->uid);
    $response[$user->name] = $res;
  }
  response($response);
  exit();
}
else {
  throw new Exception('Bad request');
}
