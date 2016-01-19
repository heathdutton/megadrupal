<?php

/**
 * @file
 *
 * Include file for MoxieManager.
 */

/**
 * Implements hook_form().
 */
function moxiemanager_settings_form($form, &$form_state) {
  // Check the module requirements
  $status = moxiemanager_check_status();

  // Get the stored settings
  $config_options = variable_get('moxiemanager_config_options', moxiemanager_config_defaults());
  $moxiemanager_plugin_config = moxiemanager_plugin_config();

  $form['installation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Installation'),
    '#collapsed' => $status,
    '#collapsible' => TRUE,
  );
  $form['installation']['licence_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Licence key'),
    '#default_value' => $moxiemanager_plugin_config['general.license'],
    '#required' => TRUE,
  );

  foreach ($config_options as $key => $default_value) {
    $form['options'][str_replace('.', '-', $key)] = array(
      '#type' => (is_bool($default_value) ? 'checkbox' : 'textfield'),
      '#title' => filter_xss($key),
      '#default_value' => $default_value,
      '#maxlength' => 255,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );

  return $form;
}

/**
 * Validation handler for moxiemanager_settings_form
 */
function moxiemanager_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['filesystem-rootpath'])) {
    file_prepare_directory($form_state['values']['filesystem-rootpath'], FILE_CREATE_DIRECTORY);
  }

  // Validate licence.
  $licence_key = $form_state['values']['licence_key'];
	if (!preg_match('/^([0-9A-Z]{4}\-){7}[0-9A-Z]{4}$/', trim($licence_key))) {
    form_set_error('licence_key', t('Invalid license: !licence_key', array('!licence_key' => $licence_key)));
	}

	$params = array(
	  'license' => $form_state['values']['licence_key'],
	  'authenticator' => 'DrupalMMAuthenticator',
	  'logged_in_key' => '',
  );
  if (!moxiemanager_install_config($params)) {
    form_set_error('licence_key', t('Could not install the configuration'));
  }
}

/**
 * Submit handler for moxiemanager_settings_form
 */
function moxiemanager_settings_form_submit($form, &$form_state) {

  // Check whether or not the settings must be reset
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    drupal_goto('admin/config/media/moxiemanager/reset');
  }

  // Replace stripes with dots
  foreach (element_children($form['options']) as $key) {
    $config[str_replace('-', '.', $key)] = $form_state['values'][$key];
  }
  variable_set('moxiemanager_config_options', $config);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Reset form
 */
function moxiemanager_settings_reset_form($form, &$form_state) {

  $form['reset'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );
  return confirm_form($form, t('Are you sure you want to reset the configuration options to their default values?'), 'admin/config/moxiemanager', t('This cannot be undone.'), t('Reset'), t('Cancel'));
}

/**
 * Submit handler for the reset form
 */
function moxiemanager_settings_reset_form_submit($form, &$form_state) {
  if ($form_state['values']['reset']) {
    variable_del('moxiemanager_config_options');
    drupal_set_message(t('The configuration options have been reset to their default values.'));
    $form_state['redirect'] = 'admin/config/media/moxiemanager';
  }
}

/**
 * Theme function for the settings form.
 */
function theme_moxiemanager_settings_form($variables) {
  $form = $variables['form'];
  foreach (element_children($form['options']) as $child) {
    // Get the form element's title
    $title = $form['options'][$child]['#title'];
    unset($form['options'][$child]['#title']);

    $rows[] = array(
      $title,
      render($form['options'][$child]),
    );
  }

  $output  = drupal_render($form['installation']);
  $output .= theme('table', array('header' => array(t('Option'), t('Value')), 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 *
 */
function moxiemanager_auth_drupal() {
  $config = variable_get('moxiemanager_config_options', moxiemanager_config_defaults());

  if (empty($config['ExternalAuthenticator.secret_key'])) {
    $output = array(
      'error' => array(
        'message' => t('No secret key set.'),
        'code' => 130,
      ),
    );
  }

  if (!isset($_REQUEST["hash"]) || !isset($_REQUEST["seed"])) {
    $output = array(
      'error' => array(
        'message' => t('Error in input.'),
        'code' => 120,
      ),
    );
  }

  $hash = !empty($_REQUEST['hash']) ? $_REQUEST['hash'] : NULL;
  $seed = !empty($_REQUEST['seed']) ? $_REQUEST['seed'] : NULL;
  $local_hash = hash_hmac('sha256', $seed, $config['ExternalAuthenticator.secret_key']);

  if ($hash === $local_hash) {
    $output = array('result' => $config);
  }
  else {
    $output = array(
      'error' => array(
        'message' => t('Error in input.'),
        'code' => 120,
      ),
    );
  }


  // We are returning JSON, so tell the browser.
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');
  print drupal_json_encode($output);
  exit(0);
}

/**
 * Command that installs the config.
 */
function moxiemanager_install_config($params) {

  $moxiemanager_root = DRUPAL_ROOT . '/' . _moxiemanager_library_directory();
	$template_path = $moxiemanager_root . '/install/config.template.php';

	if (file_exists($template_path)) {
		// Replace template variables
		$template = file_get_contents($template_path);
		foreach ($params as $key => $value) {
			$template = str_replace('<' . $key . '>', $value, $template);
		}

		if (!is_writable($moxiemanager_root . "/config.php") || !file_put_contents($moxiemanager_root . "/config.php", $template)) {
			return FALSE;
		}
	}
	return TRUE;
}
