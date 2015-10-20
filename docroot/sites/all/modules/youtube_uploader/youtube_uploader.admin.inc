<?php

/**
 * @file
 * Administrative page callbacks for the youtube_upload module.
 */

/**
 * General configuration form for youtube (account info + youtube settings).
 */
function youtube_uploader_account_settings() {

  $form['youtube_account'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Youtube account'),
  );
  $form['youtube_account']['youtube_uploader_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#description' => t('The email of your Youtube account. Example <em>xxx@gmail.com</em>'),
    '#default_value' => variable_get('youtube_uploader_username', ''),
    '#required' => TRUE,
  );
  $form['youtube_account']['youtube_uploader_passwd'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('The password of your Youtube account'),
    '#default_value' => variable_get('youtube_uploader_passwd', ''),
    '#required' => TRUE,
  );
  $form['youtube_account']['youtube_uploader_devkey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('Get a Youtube developer key !here', array('!here' => l(t('https://console.developers.google.com'), 'https://console.developers.google.com')))
      . '<br />' . t('Next in "APIs & auth" -> "Credentials" -> "Public API access" hit the button "Create a new Key" -> "Browser key".'),
    '#default_value' => variable_get('youtube_uploader_devkey', ''),
    '#required' => TRUE,
  );

  $form['youtube_settings'] = youtube_uploader_get_settings_form();

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('save configuration'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 *
 * Connect to youtube to check if account settings are correct.
 */
function youtube_uploader_account_settings_validate($form, &$form_state) {

  $vals = $form_state['values'];
  $testing = array(
    'username' => $vals['youtube_uploader_username'],
    'passwd' => $vals['youtube_uploader_passwd'],
    'devkey' => $vals['youtube_uploader_devkey'],
  );
  // Test the connection to youtube.
  module_load_include('ytapi.inc', 'youtube_uploader');
  $ytapi = new youtubeUploaderYtapi();
  $res = $ytapi->authenticate($testing);

  if (!empty($res['error'])) {
    form_set_error('youtube_account', t('Error authenticating to Youtube: !error', array('!error' => $res['error'])));
  }
  else {
    drupal_set_message(t('Connection to Youtube successful!'));
  }
}

/**
 * Implements hook_form_submit().
 */
function youtube_uploader_account_settings_submit($form, &$form_state) {
  foreach ($form_state['values'] as $key => $val) {
    // Convert settings to variables.
    if (drupal_substr($key, 0, 17) == 'youtube_uploader_') {
      variable_set($key, $val);
    }
  }
  drupal_set_message(t('Settings saved!'));
}
