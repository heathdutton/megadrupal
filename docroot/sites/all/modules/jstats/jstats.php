<?php
/**
 * @file
 *   Minimal statistic logger for jStats.
 *
 *  This file should be placed at the root of your Drupal
 *  installation, next to your index.php file. Doing this
 *  allows to log the hits on your page without fully
 *  bootstraping Drupal, only loading the minimum required
 *  files to be able to access the database.
 */

// Output headers & data and close connection
ignore_user_abort(TRUE);
ob_start();
header("Content-type: text/javascript; charset=utf-8");
header("Expires: Sun, 19 Nov 1978 05:00:00 GMT");
header("Cache-Control: no-cache");
header("Cache-Control: must-revalidate");
header("Content-Length: 13");
header("Connection: close");
print("/* jstats */\n");
@ob_end_flush();
@ob_flush();
@flush();

define('DRUPAL_ROOT', getcwd());
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

// Manually include common.inc to get access to drupal_write_record.
require_once './includes/common.inc';

// Can't use module_load_include as it's not loaded yet, so fallback to
// drupal_get_filename to find our module.
$path = dirname(drupal_get_filename('module', 'jstats'));
require_once $path .'/jstats.inc';

jstats_callback(TRUE);
