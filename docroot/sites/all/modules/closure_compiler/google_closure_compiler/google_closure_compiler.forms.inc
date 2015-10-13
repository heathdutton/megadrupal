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
function _google_closure_compiler_form_system_performance_settings_alter() {
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.checks');
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');

  $element = array();

  // If local compiling option is set, make sure it is still operational.
  google_closure_compiler_confirm_local(TRUE);

  $element['closure_compiler_local_status'] = array(
    '#type' => 'item',
    '#title' => t('Local Closure Compiler Status'),
    '#markup' => _google_closure_compiler_local_status_content(),
  );

  $local_option = t('Compile locally via Java based compiler');
  $process_default = variable_get('google_closure_compiler_process_method', CLOSURE_COMPILER_SEND_CONTENTS);
  $local_available_js_setting = TRUE;
  if (!google_closure_compiler_local_works()) {
    $local_option .= ' ' . t('(Not available due to error(s))');
    if (CLOSURE_COMPILER_LOCAL == $process_default) {
      $process_default = CLOSURE_COMPILER_SEND_CONTENTS;
    }
    $local_available_js_setting = FALSE;
  }
  $element['google_closure_compiler_process_method'] = array(
    '#type' => 'radios',
    '#title' => t('Preferred Processing Method'),
    '#default_value' => $process_default,
    '#options' => array(
      CLOSURE_COMPILER_LOCAL => $local_option,
      CLOSURE_COMPILER_SEND_CONTENTS => t('Send JavaScript file contents to the API'),
      CLOSURE_COMPILER_SEND_URL => t('Send JavaScript file paths to the API (Requires your site to be publicly accessible by Google)'),
    ),
    '#description' => t("'Send file contents' option reads the cached JavaScript files under %filesyspath and sends the code directly to the service to be compiled.<br />'Send file paths' option sends the URI of the JavaScript file for its contents to be retrieved and compiled by the service. Your site's URLs must be public for this method to work.<br />NOTE: If your JavaScript payload size is bigger than 200000 bytes, the module uses the third method in place of the second due to an API limitation.", array('%filesyspath' => file_default_scheme() . '://js')),
  );

  $element['google_closure_compiler_compilation_level'] = array(
    '#type' => 'radios',
    '#title' => t('Compilation Level'),
    '#default_value' => variable_get('google_closure_compiler_compilation_level', GOOGLE_CLOSURE_COMPILER_SIMPLE),
    '#options' => array(
      GOOGLE_CLOSURE_COMPILER_WHITESPACE => t('Whitespace only'),
      GOOGLE_CLOSURE_COMPILER_SIMPLE => t('Simple optimizations'),
      GOOGLE_CLOSURE_COMPILER_ADVANCED => t('Advanced optimizations (Not recommended)'),
    ),
    '#description' => t('The degree of compression and optimization to apply to your JavaScript. Check out the !link for more information. Advanced optimizations level tends to break Drupal JavaScript and tends to be highly unstable. If unsure, keep the default setting. A change to this setting may require a cache clear to fully go into effect.', array('!link' => l('API reference page', 'http://code.google.com/closure/compiler/docs/api-ref.html'))),
  );

  drupal_add_js(array('google_closure_compiler' => array('local_available' => $local_available_js_setting)), 'setting');
  drupal_add_js(drupal_get_path('module', 'google_closure_compiler') . '/system_performance_settings.min.js', 'file');

  return $element;
}

/**
 * Generates and returns the local closure compiler status markup.
 *
 * Used in the system_performance_settings form.
 */
function _google_closure_compiler_local_status_content() {
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.checks');
  module_load_include('inc', 'google_closure_compiler', 'google_closure_compiler.defines');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');

  $java_installed = closure_compiler_java_installed();
  $java_status = array(
    'value' => $java_installed ? 'Java appears to be installed on your system.' : 'Unable to verify that Java is installed via command line PHP.',
    'title' => 'Java',
    'severity' => $java_installed ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $compiler_exists = google_closure_compiler_compiler_exists();
  $module_path = drupal_get_path('module', 'google_closure_compiler');
  $compiler_status = array(
    'value' => $compiler_exists ? 'Compiler file ' . GOOGLE_CLOSURE_COMPILER_FILE . ' exists in Google Closure Compiler module directory.' : 'Unable to locate ' . GOOGLE_CLOSURE_COMPILER_FILE . ' in Google Closure Compiler module directory (' . $module_path . ').',
    'title' => 'Compiler File',
    'severity' => $compiler_exists ? REQUIREMENT_OK : REQUIREMENT_ERROR,
  );

  $compiler_working = google_closure_compiler_local_works();
  $compiler_working_status = array(
    'value' => $compiler_working ? 'Verified that Closure Compiler works locally.' : 'Unable to run Closure Compiler locally. Please check Java version, file permissions.',
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
