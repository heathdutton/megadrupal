<?php
/**
 * @file
 * Adminstration functions for BitTorrent Sync API module.
 */

/**
 * Settings form callback.
 */
function btsync_form($form, $form_state) {
  $settings = variable_get('btsync_settings', array());

  $form['btsync_settings'] = array(
    '#title' => t('API settings'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => FALSE,
  );

  $form['btsync_settings']['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['url']) ? $settings['url'] : 'http://127.0.0.1:8888/api',
    '#required' => TRUE,
  );

  $form['btsync_settings']['username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['username']) ? $settings['username'] : NULL,
    '#required' => TRUE,
  );

  $form['btsync_settings']['password'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    '#default_value' => isset($settings['password']) ? $settings['password'] : NULL,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
