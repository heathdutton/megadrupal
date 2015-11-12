<?php

/**
 * @file
 * Drupal bootstrap.
 */

$string = dirname(__FILE__);
$path = substr($string,0,strlen($string)-40);
chdir($path);
define('DRUPAL_ROOT', $path);
require_once DRUPAL_ROOT . 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
