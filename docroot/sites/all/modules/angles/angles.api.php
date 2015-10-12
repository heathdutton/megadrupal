<?php

/**
 * @file
 * Hooks provided by Angles.
 */

/**
 * Implements hook_angles_controllers().
 *
 * file can either be a string or an array of strings for
 * multiple js loading.
 *
 * If you change the machine name each block that uses that machine name
 * must be reset (this will also clear the cache for that block).
 *
 * If you just change the file path you must manually clear cache.
 */
function hook_angles_controllers() {
  return array(
    array(
      'machine_name' => 'TodoCtrl',
      'human_name' => 'Todo',
      'file' => 'sites/all/modules/example/test.js',
    ),
  );
}

/**
 * Implements hook_angles_requires_js().
 *
 * If you just change the file path you must manually clear cache.
 */
function hook_angles_require_js() {
  return array(
    array(
      'sites/all/modules/example/test.js',
    ),
  );
}

/**
 * Implements hook_angles_templates().
 *
 * If you change the machine name each block that uses that machine name
 * must be reset (this will also clear the cache for that block).
 *
 * If you just change the file path you must manually clear cache.
 */
function hook_angles_templates() {
  return array(
    array(
      'machine_name' => 'texample',
      'human_name' => 'Template Example',
      'file' => 'sites/all/modules/custom/example/example.html',
    ),
  );
}
