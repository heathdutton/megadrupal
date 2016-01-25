<?php

/**
 * @file
 * API documentation for QUnit.
 */

/**
 * Implements hook_library_alter().
 */
function mymodule_library_alter(&$libraries, $module) {
  // Add available JavaScript tests and dependencies.
  if ($module == 'qunit') {
    $libraries['qunit']['js'][drupal_get_path('module', 'mymodule') . '/mymodule.test.js'] = array();
  }
}
