<?php
/**
 * @file
 * Administrative page code for the sms module.
 */

/**
 * Administrative settings.
 */
function notify_slack_admin_settings() {
  $form['notify_slack_default_hook_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default Hook URL'),
    '#default_value' => variable_get('notify_slack_default_hook_url', ''),
  );
  $form['notify_slack_default_channel'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default Channel'),
    '#default_value' => variable_get('notify_slack_default_channel', '#random'),
  );
  $form['notify_slack_default_bot_name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default Bot Name'),
    '#default_value' => variable_get('notify_slack_default_bot_name', 'Shrek'),
  );

  return system_settings_form($form);
}