<?php

/**
 * @file
 * iMIS Webservices administrative page callbacks.
 */

/**
 * Config form for setting iMIS endpoint.
 */
function imis_ws_config_form() {
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Configuration'),
  );

  $form['general']['imis_ws_host'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Host'),
    '#default_value' => variable_get('imis_ws_host'),
    '#description'   => t('The host name to authenticate against. You must include the url scheme <em>e.g. http://</em>. Do not include a trailing <em>/</em>'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
