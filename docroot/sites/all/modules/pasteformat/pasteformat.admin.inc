<?php
function pasteformat_settings_form() {
  $form = array();
  foreach (filter_formats() as $key => $format) {
    $options[$key] = $format->name;
  }
  
  $form['pasteformat_cleanup_format'] = array(
    '#type' => 'radios',
    '#title' => t('Text format to clean up pasted text'),
    '#options' => $options,
    '#default_value' => variable_get('pasteformat_cleanup_format', filter_fallback_format()),
  );
  $form['pasteformat_message_success'] = array(
    '#type' => 'textfield',
    '#title' => t('Clean-up alert'),
    '#default_value' => variable_get('pasteformat_message_success', ''),
    '#description' => t('Message to show to the users after text clean-up. Leave it empty to disable alert.'),
  );
  /*$form['pasteformat_message_failure'] = array(
    '#type' => 'textfield',
    '#title' => t('Failure alert'),
    '#default_value' => variable_get('pasteformat_message_failure', ''),
    '#description' => t('Message to show to the users in case of failure. Leave it empty to disable alert.'),
  );*/
  
  return system_settings_form($form);
}
