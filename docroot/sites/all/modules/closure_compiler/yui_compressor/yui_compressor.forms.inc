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
function _yui_compressor_form_system_performance_settings_alter() {
  module_load_include('inc', 'yui_compressor', 'yui_compressor.checks');

  $element = array();

  // Make sure local compiling is still operational.
  yui_compressor_confirm_local(TRUE);

  $element['yui_compressor_local_status'] = array(
    '#type' => 'item',
    '#title' => t('Local YUI Compressor Status'),
    '#markup' => _yui_compressor_local_status_content(),
  );

  drupal_add_js(drupal_get_path('module', 'yui_compressor') . '/system_performance_settings.min.js', 'file');

  return $element;
}

/**
 * Generates and returns the local closure compiler status markup.
 *
 * Used in the system_performance_settings form.
 */
function _yui_compressor_local_status_content() {
  module_load_include('inc', 'yui_compressor', 'yui_compressor.checks');
  module_load_include('inc', 'yui_compressor', 'yui_compressor.defines');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');

  $java_installed = closure_compiler_java_installed();
  $java_status = array(
    'value' => $java_installed ? 'Java appears to be installed on your system.' : 'Unable to verify that Java is installed via command line PHP.',
    'title' => 'Java',
    'severity' => $java_installed ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $compiler_exists = yui_compressor_compiler_exists();
  $module_path = drupal_get_path('module', 'yui_compressor');
  $compiler_status = array(
    'value' => $compiler_exists ? 'Compiler file ' . YUI_COMPRESSOR_FILE . ' exists in YUI Compressor module directory.' : 'Unable to locate ' . YUI_COMPRESSOR_FILE . ' in YUI Compressor module directory (' . $module_path . ').',
    'title' => 'Compiler File',
    'severity' => $compiler_exists ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $compiler_working = yui_compressor_local_works();
  $compiler_working_status = array(
    'value' => $compiler_working ? 'Verified that YUI Compressor works locally.' : 'Unable to run YUI Compressor locally. Please check Java version, file permissions.',
    'title' => 'Local Compiler Test',
    'severity' => $compiler_working ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $requirements = array(
    $java_status,
    $compiler_status,
    $compiler_working_status,
  );

  return theme('status_report', array('requirements' => $requirements));
}
