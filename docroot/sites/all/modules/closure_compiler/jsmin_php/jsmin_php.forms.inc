<?php

/**
 * @file
 * Contains the form-related functions.
 */

/**
 * Returns fields to add to the system_performance_settings form.
 *
 * Used to display the sub-module settings.
 */
function _jsmin_php_form_system_performance_settings_alter() {
  module_load_include('inc', 'jsmin_php', 'jsmin_php.checks');

  $element = array();

  // Make sure local compiling is still operational.
  jsmin_php_confirm_local(TRUE);

  $element['jsmin_php_local_status'] = array(
    '#type' => 'item',
    '#title' => t('Local JSMin-PHP Status'),
    '#markup' => _jsmin_php_local_status_content(),
  );

  drupal_add_js(drupal_get_path('module', 'jsmin_php') . '/system_performance_settings.min.js', 'file');

  return $element;
}

/**
 * Generates and returns the local closure compiler status markup.
 *
 * Used in the system_performance_settings form.
 */
function _jsmin_php_local_status_content() {
  module_load_include('inc', 'jsmin_php', 'jsmin_php.checks');

  $compiler_exists = jsmin_php_compiler_exists();
  $compiler_status = array(
    'value' => $compiler_exists ? 'Compiler file jsmin.php exists in jsmin-php library directory.' : 'Unable to locate jsmin.php in jsmin-php library directory (usually sites/all/libraries/jsmin-php/).',
    'title' => 'Compiler File',
    'severity' => $compiler_exists ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $compiler_working = jsmin_php_local_works();
  $compiler_working_status = array(
    'value' => $compiler_working ? 'Verified that JSMin-PHP works locally.' : 'Unable to run JSMin-PHP locally. Please check file permissions.',
    'title' => 'Local Compiler Test',
    'severity' => $compiler_working ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $requirements = array(
    $compiler_status,
    $compiler_working_status,
  );

  return theme('status_report', array('requirements' => $requirements));
}
