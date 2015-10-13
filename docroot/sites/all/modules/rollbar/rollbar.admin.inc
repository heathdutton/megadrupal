<?php
/**
 * @file rollbar.admin.inc.
 */


/**
 * Admin form callback for rollbar settings
 */
function rollbar_admin_settings_form($form, &$form_state) {
  $form = array();

  // Common settings
  $form['common'] = array(
    '#type' => 'fieldset',
    '#title' => t('Common'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['common']['rollbar_environment'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Environment'),
    '#description' => t('Specify the environment name to be sent to the
        Rollbar API.'),
    '#default_value' => variable_get('rollbar_environment', 'production'),
  );

  // PHP settings
  $form['php'] = array(
    '#type' => 'fieldset',
    '#title' => t('PHP'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['php']['rollbar_enable_php'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Rollbar for PHP.'),
    '#default_value' => variable_get('rollbar_enable_php', 1),
  );
  $form['php']['rollbar_lib_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Library path'),
    '#description' => t('This is the path to the rollbar.php. Relative to the
        Drupal install.'),
    '#default_value' => variable_get(
      'rollbar_lib_path',
      'sites/all/libraries/rollbar/rollbar.php'
    ),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_php"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['php']['rollbar_server_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token'),
    '#description' => t('This is the token labeled post_server_item.'),
    '#default_value' => variable_get('rollbar_server_access_token', NULL),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_php"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['php']['rollbar_exceptions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Register Exception Handler.'),
    '#default_value' => variable_get('rollbar_exceptions', 1),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_php"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['php']['rollbar_error_handling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Register Error Handler.'),
    '#default_value' => variable_get('rollbar_error_handling', 1),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_php"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['php']['rollbar_watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include watchdog messages.'),
    '#default_value' => variable_get('rollbar_watchdog', 0),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_php"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['php']['rollbar_watchdog_include_php'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send watchdog PHP errors and exceptions.'),
    '#enabled' => FALSE,
    '#description' => t('Watchdog PHP errors will most likely include duplicates
        so should probably be turned off, unless you\'ve opted out of
        registering as an Error and Exception handler above.'),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_php"]' => array('checked' => FALSE),
      ),
      'enabled' => array(
        'input[name="rollbar_watchdog"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('rollbar_watchdog_include_php', 0),
  );

  // client settings for managing JavaScript events
  $form['js'] = array(
    '#type' => 'fieldset',
    '#title' => t('JavaScript'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('If you want Rollbar to collect JavaScript errors.
        Then configure the client settings below.'),
  );
  $form['js']['rollbar_enable_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Rollbar for JavaScript.'),
    '#default_value' => variable_get('rollbar_enable_js', 1),
  );
  $form['js']['rollbar_js_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token'),
    '#description' => t('This is the token labeled post_client_item.'),
    '#default_value' => variable_get('rollbar_js_access_token', NULL),
    '#states' => array(
      'disabled' => array(
        ':input[name="rollbar_enable_js"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['#validate'][] = 'rollbar_admin_settings_form_validate';
  $form = system_settings_form($form);
  return $form;
}


/**
 * Form validation function for rollbar_admin_settings_form().
 */
function rollbar_admin_settings_form_validate($form, &$form_state) {

  // only validate if PHP is enabled, we don't really care otherwise
  if ($form_state['values']['rollbar_enable_php'] == 1) {
    if (!preg_match("/^[0-9a-z]{32}$/i", $form_state['values']['rollbar_server_access_token'])) {
      form_set_error(
        'rollbar_server_access_token',
        'You must specify a valid Rollbar token.'
      );
    }

    // rollbar library must exist
    if (!file_exists($form_state['values']['rollbar_lib_path'])) {
      form_set_error('rollbar_lib_path', 'Couldn\'t find the Rollbar library
           in the specified path.');
    }
  }

  // ensure key is provided for JS
  if ($form_state['values']['rollbar_enable_js'] == 1) {

    // make sure it's alphanumeric
    if (!preg_match("/^[0-9a-z]{32}$/i", $form_state['values']['rollbar_js_access_token'])) {
      form_set_error(
        'rollbar_js_access_token',
        'You must specify a valid Rollbar token.'
      );
    }


  }
}
