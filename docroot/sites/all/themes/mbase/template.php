<?php
/**
 * @file
 * Theme template.php file.
 */

// Ensure that __DIR__ constant is defined.
if (!defined('__DIR__')) {
  define('__DIR__', dirname(__FILE__));
}

// Include all require files.
require_once __DIR__ . '/includes/helper.inc';
require_once __DIR__ . '/includes/bootstrap.inc';

drupal_static_reset('element_info');
