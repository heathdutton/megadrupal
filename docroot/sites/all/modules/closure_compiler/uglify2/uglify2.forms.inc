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
function _uglify2_form_system_performance_settings_alter() {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  $element = array();

  $element['uglify2_process_method'] = array(
    '#type' => 'radios',
    '#title' => t('Preferred Processing Method'),
    '#default_value' => variable_get('uglify2_process_method', CLOSURE_COMPILER_SEND_CONTENTS),
    '#options' => array(
      CLOSURE_COMPILER_SEND_CONTENTS => t('Send JavaScript file contents to the API'),
      CLOSURE_COMPILER_SEND_URL => t('Send JavaScript file paths to the API (Requires your site to be publicly accessible)'),
    ),
    '#description' => t("'Send file contents' option reads the cached JavaScript files under %filesyspath and sends the code directly to the service to be compiled.<br />'Send file paths' option sends the URI of the JavaScript file for its contents to be retrieved and compiled by the service. Your site's URLs must be public for this method to work.<br />NOTE: If your JavaScript payload size is bigger than 200000 bytes, the module uses the second method in place of the first due to an API limitation.", array('%filesyspath' => file_default_scheme() . '://js')),
  );

  drupal_add_js(drupal_get_path('module', 'uglify2') . '/system_performance_settings.min.js', 'file');

  return $element;
}
