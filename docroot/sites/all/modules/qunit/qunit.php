<?php

/**
 * @file
 * Dummy PHP file that bootstraps the Drupal administration page to work around
 * a QUnit bug where we can't have ?q=... in the URL.
 */

$script_name = $_SERVER['SCRIPT_NAME'];
$script_parts = explode('/', $script_name);
array_pop($script_parts);
while (!file_exists('./index.php')) {
  chdir('..');
  array_pop($script_parts);
}

$_SERVER['SCRIPT_NAME'] = implode('/', $script_parts) . '/index.php';
$_GET['q'] = 'admin/config/development/qunit';
$qunit_hack = TRUE;
require_once('./index.php');
