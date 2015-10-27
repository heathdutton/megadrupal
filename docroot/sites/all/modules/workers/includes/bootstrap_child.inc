<?php
/**
 * @file
 */

$_SERVER['REMOTE_ADDR'] = '127.0.0.1';

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

// Prevent Devel from hi-jacking our output in any case.
$GLOBALS['devel_shutdown'] = FALSE;

require_once DRUPAL_ROOT . '/includes/unicode.inc';
unicode_check();

require_once DRUPAL_ROOT . '/includes/module.inc';
require_once DRUPAL_ROOT . '/includes/file.inc';
file_get_stream_wrappers();
