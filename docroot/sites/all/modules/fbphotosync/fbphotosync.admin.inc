<?php
/**
 * @file
 */

/**
 * Administration form.
 */
function fbphotosync_admin($form, $form_state, $reset = FALSE) {
  $settings = isset($form_state['input']['fbphotosync_settings'])
    ? $form_state['input']['fbphotosync_settings']
    : variable_get('fbphotosync_settings', array());
  $form['fbphotosync_settings'] = array(
    '#tree' => TRUE,
  );

  $form['fbphotosync_settings']['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => isset($settings['app_id']) ? $settings['app_id'] : '',
    '#required' => TRUE,
  );

  $form['fbphotosync_settings']['app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App secret'),
    '#default_value' => isset($settings['app_secret']) ? $settings['app_secret'] : '',
    '#required' => TRUE,
  );

  if (isset($settings['app_id']) && isset($settings['app_secret']) && $facebook = fbphotosync_facebook_sdk()) {
    // Authenticate Facebook account.
    if ($reset || empty($settings) || !isset($settings['access_token']) || (isset($settings['access_token']) && !fbphotosync_facebook_sdk_is_authenticated($facebook, $settings['access_token']))) {
      if (isset($_GET['code'])) {
        $settings['access_token'] = $facebook->getAccessToken();
        variable_set('fbphotosync_settings', $settings);
        drupal_goto('admin/config/services/fbphotosync');
      }
      else {
        unset($settings['access_token']);
        variable_set('fbphotosync_settings', $settings);
        if (!isset($_SESSION[md5(serialize($settings))])) {
          $_SESSION[md5(serialize($settings))] = FALSE;
          drupal_goto($facebook->getLoginUrl(array('scope' => array('manage_pages', 'offline_access', 'publish_stream', 'user_photos'))));
        }
        else {
          unset($_SESSION[md5(serialize($settings))]);
          form_set_error('fbphotosync_settings', 'Authentication failed');
          return system_settings_form($form);
        }
      }
    }

    if (!empty($settings)) {
      $form['fbphotosync_settings']['access_token'] = array(
        '#type' => 'value',
        '#value' => $settings['access_token'],
      );

      $form['fbphotosync_settings']['authenticated'] = array(
        '#type' => 'fieldset',
        '#title' => t('Facebook user'),
        '#description' => theme('fbphotosync_authenticated_user', array('account' => fbphotosync_facebook_sdk_get($facebook, '/me'))),
      );
    }
  }

  return system_settings_form($form);
}
