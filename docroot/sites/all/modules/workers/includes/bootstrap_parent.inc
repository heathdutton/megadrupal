<?php
/**
 * @file
 */

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);

// Prevent Devel from hi-jacking our output in any case.
$GLOBALS['devel_shutdown'] = FALSE;

// Deactivate Drupal Error and Exception handling.
restore_error_handler();
restore_exception_handler();
