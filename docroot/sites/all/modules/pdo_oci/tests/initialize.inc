<?php

// Clone from index.php.
// This file is under includes/database/oci/debug/includes
define('DRUPAL_ROOT', getcwd() . '/../../../..');

// Add simpletest-like assert support for debug.
require_once dirname(__FILE__) . '/drupal_web_test_case.inc';
require_once dirname(__FILE__) . '/bootstrap.inc';

// Include some common *.inc.
require_once DRUPAL_ROOT . '/includes/unicode.inc';

// Clone from drupal_bootstrap().
// Just initialize basic settings.
_drupal_bootstrap_configuration();

// Fake the database connection setting to sample database.
if (strpos(dirname(__FILE__), 'mysql')) {
  require_once dirname(__FILE__) . '/settings.mysql.php';
}
elseif (strpos(dirname(__FILE__), 'oci')) {
  require_once dirname(__FILE__) . '/settings.oci.php';
}

// Clone from _drupal_bootstrap_database().
// We don't call _drupal_bootstrap_database() directly since don't need to
// pass to installer or from simpletest.
require_once DRUPAL_ROOT . '/includes/database/database.inc';
spl_autoload_register('drupal_autoload_class');
spl_autoload_register('drupal_autoload_interface');
