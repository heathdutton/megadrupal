<?php

/**
 * @file
 * Presents the admin interface for the module.
 */

/**
 * The actual settings form in admin/config/development/javascript-drupal-extension.
 */
function javascript_drupal_extension_config() {
  $form = array();

  $form['javascript_drupal_extension_ajax_callback_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('AJAX Callback URL'),
    '#description'   => t('Edit the URL used for AJAX callbacks from the JavaScript library.'),
    '#default_value' => variable_get('javascript_drupal_extension_ajax_callback_url', 'javascript-drupal-extension'),
    '#required'      => TRUE,
  );
  $form['javascript_drupal_extension_function_whitelist'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Function whitelist'),
    '#description'   => t('Add a list of functions (one function pr line) that are allowed to be called through the AJAX calls.'),
    '#default_value' => variable_get('javascript_drupal_extension_function_whitelist', "drupal_get_path\r\nwatchdog"),
  );
  $form['javascript_drupal_extension_parse_whitelist'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Verify the function through the whitelist (highly recommended)'),
    '#description'   => t('Toggles wether or not the whitelist will be used to screen functions to be called. It is highly recommended that you leave this setting on.'),
    '#default_value' => variable_get('javascript_drupal_extension_parse_whitelist', TRUE),
  );
  $form['javascript_drupal_extension_use_minified_js'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use the minified version of the JavaScript file (recommended)'),
    '#description'   => t('Toggles wether or not the minified version of the JavaScript file is to be used. The advantages of using a minified file is mainly to save bandwidth.'),
    '#default_value' => variable_get('javascript_drupal_extension_use_minified_js', TRUE),
  );
  $form['javascript_drupal_extension_allow_get_variables'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Allow parsing of _GET'),
    '#description'   => t('Allow the main execute function in PHP to fetch parameters to the call-function from _GET (default is just from _POST).'),
    '#default_value' => variable_get('javascript_drupal_extension_allow_get_variables', FALSE),
  );
  $form['javascript_drupal_extension_parse_output_as_json'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Parse "Output-as-JSON"'),
    '#description'   => t('Wether or not to parse "Output-as-JSON" if it is passed as a variable to the AJAX call. If not it will be passed as a variable to the calling PHP/Drupal function.'),
    '#default_value' => variable_get('javascript_drupal_extension_parse_output_as_json', TRUE),
  );
  $form['javascript_drupal_extension_parse_content_type'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Parse "Content-Type"'),
    '#description'   => t('Wether or not to parse "Content-Type" if it is passed as a variable to the AJAX call. If not it will be passed as a variable to the calling PHP/Drupal function.'),
    '#default_value' => variable_get('javascript_drupal_extension_parse_content_type', TRUE),
  );
  $form['javascript_drupal_extension_log_usage_to_watchdog'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Log usage to watchdog'),
    '#description'   => t('Log to watchdog every time a function is called from JavaScript.'),
    '#default_value' => variable_get('javascript_drupal_extension_log_usage_to_watchdog', FALSE),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function javascript_drupal_extension_config_validate($form, &$form_state) {
  $functions = array();
  $whitelist = $form_state['values']['javascript_drupal_extension_function_whitelist'];

  if (strpos($whitelist, chr(13) . chr(10)) !== FALSE) {
    $functions = explode(chr(13) . chr(10), $whitelist);
  }
  elseif (strpos($whitelist, chr(13)) !== FALSE) {
    $functions = explode(chr(13), $whitelist);
  }
  else {
    $functions = explode(chr(10), $whitelist);
  }

  if (is_array($functions)) {
    foreach ($functions as $function) {
      if (!function_exists($function)) {
        form_set_error(
          'javascript_drupal_extension_function_whitelist',
          t('One of the functions you are trying to whitelist doesn\'t exist: !function', array(
            '!function' => $function,
          )));
      }
    }
  }

  drupal_flush_all_caches();
}
