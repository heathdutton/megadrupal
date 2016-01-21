<?php

/**
 * @file
 *   Administrative pages for the stingray module.
 */

/**
 * General Settings for stingray
 */
function stingray_settings() {
  $form = array();

  $form['stingray_server'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server URL'),
    '#default_value' => variable_get('stingray_server', ''),
    '#description'   => t('The URL of the Stingray server. e.g. "https://example.com:9090/soap')
  );
  $form['stingray_server_username'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server username'),
    '#default_value' => variable_get('stingray_server_username', ''),
    '#description'   => t('Stingray server username')
  );
  $form['stingray_server_password'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server password'),
    '#default_value' => variable_get('stingray_server_password', ''),
    '#description'   => t('Stingray server password')
  );
  $form['stingray_wsdl_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('SOAP WSDL'),
    '#default_value' => variable_get('stingray_wsdl_dir', drupal_get_path('module', 'stingray') . '/wsdl'),
    '#description'   => t('The URL of the stingray SOAP call WSDL, e.g. "https://soap.example.com/example.wsdl" or relative path')
  );
  $form['stingray_base_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Base path'),
    '#default_value' => variable_get('stingray_base_path'),
    '#description'   => t('The URL of the CND server, e.g. "https://cnd.example.com"')
  );
  return system_settings_form($form);
}

/**
 * General cache clearing page.
 */
function stingray_cache_control() {
  $form = array();
  $form['paths'] = array(
    '#type'        => 'textarea',
    '#title'       => t('Paths/URLs'),
    '#description' => t('Enter one URL per line. URL entries should be relative to the basepath. (e.g. node/1, content/pretty-title, sites/default/files/some/image.png'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Start Refreshing Content'),
  );

  return $form;
}


/**
 * Submit handler for stingray_cache_control().
 *
 * Process the settings and submit to stingray
 */
function stingray_cache_control_submit($form, &$form_state) {
  // Get path per line
  $paths = explode("\r\n", filter_xss($form_state['values']['paths']));

  $has_clear = stingray_clear_url($paths);
  if ($has_clear) {
    $message = t("Stingray cache request has been made successfully.") . theme("item_list", $paths);
    drupal_set_message($message);
  }
  else {
    drupal_set_message(t("There was a problem with your cache control request.  Check your log messages for more information."), 'error' );
  }
}

