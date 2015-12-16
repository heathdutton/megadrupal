<?php

function pax_content_admin_settings() {
  $form = array();

  $form['pax_content_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('pax_content_api_key', ''),
    '#description' => 'An API key is needed to get access to PaX content API.',
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
