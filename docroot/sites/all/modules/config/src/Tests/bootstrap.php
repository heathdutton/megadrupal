<?php

/**
 * @file
 * Special bootrap for PHP testing.
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

// We assume that PHPUnit is run from the Tests directory in
// sites/all/modules/config.
define('DRUPAL_ROOT', './../../../../..');
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';

// Bootstrap Drupal.
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

define('CONFIG_TEST_ROOT', getcwd());
