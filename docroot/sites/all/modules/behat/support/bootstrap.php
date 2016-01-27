<?php
$_SERVER['SCRIPT_NAME'] = '/ClosuredFeatureContext.php';
$_SERVER['SCRIPT_FILENAME'] = '/ClosuredFeatureContext.php';
$_SERVER['HTTP_HOST'] = 'localhost';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['REQUEST_METHOD'] = 'POST';

define('DRUPAL_ROOT', getcwd() );
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

require_once DRUPAL_ROOT .'/modules/simpletest/drupal_web_test_case.php';