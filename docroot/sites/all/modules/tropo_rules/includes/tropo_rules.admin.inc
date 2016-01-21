<?php
/**
 * @file
 * Administration settings for the Tropo API rules integration.
 */
/**
 * Settings Form
 */
function tropo_rules_settings_form($form, &$form_state) {
  $form['tropo_rules_api_messaging_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Messaging Token'),
    '#description' => t('These are assigned to you when you create your application on Tropo.com.'),
    '#maxlength' => 88,
    '#size' => 88,
    '#default_value' => variable_get('tropo_rules_api_messaging_token'),
  );
  $form['tropo_rules_api_voice_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Voice Token'),
    '#description' => t('These are assigned to you when you create your application on Tropo.com.'),
    '#maxlength' => 88,
    '#size' => 88,
    '#default_value' => variable_get('tropo_rules_api_voice_token'),
  );

  return system_settings_form($form);
}
