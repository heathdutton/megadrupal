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
  $form['notify_slack_default_avatar'] = array(
    '#type'          => 'radios',
    '#options'       => array(
      'icon_emoji' => t('Icon Emoji'),
      'icon_url'   => t('Icon URL'),
    ),
    '#title'         => t('Bots avatar'),
    '#default_value' => variable_get('notify_slack_default_avatar', 'icon_emoji'),
  );
  $form['notify_slack_default_icon_emoji'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default icon emoji'),
    '#default_value' => variable_get('notify_slack_default_icon_emoji', ':ghost:'),
    '#states'        => array(
      'visible'  => array(
        ':input[name="notify_slack_default_avatar"]' => array('value' => 'icon_emoji'),
      ),
      'required' => array(
        ':input[name="notify_slack_default_avatar"]' => array('value' => t('icon_emoji')),
      ),
    ),
  );
  $form['notify_slack_default_icon_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Default icon URL'),
    '#default_value' => variable_get('notify_slack_default_icon_url', ''),
    '#states'        => array(
      'visible'  => array(
        ':input[name="notify_slack_default_avatar"]' => array('value' => 'icon_url'),
      ),
      'required' => array(
        ':input[name="notify_slack_default_avatar"]' => array('value' => t('icon_url')),
      ),
    ),
  );

  return system_settings_form($form);
}
