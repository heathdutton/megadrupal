<?php

function message_docs_admin_settings() {
  $form['message_docs_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => variable_get('message_docs_site_id', ''),
    '#required' => FALSE,
  );

  $form['message_docs_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get('message_docs_server_url', ''),
    '#required' => FALSE,
  );

  $form['message_docs_helper_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status messages informing about the module'),
    '#default_value' => variable_get('message_docs_helper_messages', ''),
    '#required' => FALSE,
  );
  return system_settings_form($form);
}


