<?php
/**
 * Bootstrapping File for ssh_helper Test Suite
 */

// Without the next 5 lines our php unit won't work
// Define our Drupal Root
// depends where you run it from. Have not found a way to make it run dynamically without telling drupal where it's root
// is. 
define('DRUPAL_ROOT', 'docroot/');

// Define our remote addr
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';

// Start Drupal
require_once DRUPAL_ROOT . 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

module_enable(array('ssh_helper'));
