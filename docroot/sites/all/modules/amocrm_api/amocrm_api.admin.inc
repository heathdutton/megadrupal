<?php

/**
 * @file
 * Administrative page callbacks for the amocrm_api module.
 */

/**
 * General configuration form for controlling the amocrm_api behaviour.
 */
function amocrm_api_admin_settings() {
  $form['amocrm_api_default_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Default domain'),
    '#default_value' => variable_get('amocrm_api_default_domain'),
  );

  $form['amocrm_api_default_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Default email'),
    '#default_value' => variable_get('amocrm_api_default_email'),
  );

  $form['amocrm_api_default_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Default api key'),
    '#default_value' => variable_get('amocrm_api_default_key'),
  );

  return system_settings_form($form);
}
