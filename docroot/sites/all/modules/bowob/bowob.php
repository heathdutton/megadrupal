<?php
// $Id$

/**
 * @file bowob.php
 *
 * Entry point for BoWoB server and client.
 */

// Next two lines depend on the path of this file: /sites/all/modules/bowob/bowob.php
$_SERVER['SCRIPT_NAME'] = $_SERVER['PHP_SELF'] = substr($_SERVER['SCRIPT_NAME'], 0, -33) . 'index.php';
define('DRUPAL_ROOT', dirname(dirname(dirname(dirname(dirname(realpath(__FILE__)))))));

require(DRUPAL_ROOT . '/includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

module_load_include('php', 'bowob', 'bowob_functions');

$bowob_type = is_numeric(@$_GET['type']) ? intval($_GET['type']) : -1;

if($bowob_type == 2)
{
  bowob_server_sync();
}
elseif($bowob_type == 3)
{
  bowob_client_sync();
}
elseif($bowob_type == 4)
{
  bowob_redirect_login();
}
elseif($bowob_type == 5)
{
  bowob_redirect_profile();
}

?>
