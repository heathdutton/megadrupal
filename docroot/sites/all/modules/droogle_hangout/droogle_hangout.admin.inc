<?php

/**
 * @file
 * Google Calendar api connection
 * Admin settings.
 */

/**
 * Uses the form api for an admin settings form.
 */
function droogle_hangout_settings_form() {
  global $user;
  global $base_url;

  $droogle_hangout_master_account_email = variable_get('droogle_hangout_master_account_email', '');
  $droogle_hangout_clientid = variable_get('droogle_hangout_clientid', '');
  $droogle_hangout_client_secret = variable_get('droogle_hangout_client_secret', '');
  $droogle_hangout_refresh_token = variable_get('droogle_hangout_refresh_token', '');
  $droogle_hangout_custom_css = variable_get('droogle_hangout_custom_css', '');

  $form['droogle_hangout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for Google Console'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['droogle_hangout']['droogle_hangout_master_account_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address connecting to the Google Console.'),
    '#default_value' => $droogle_hangout_master_account_email,
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t('Enter the email address used for your Google Console that is handling the api.'),
  );

  $form['droogle_hangout']['droogle_hangout_clientid'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Console Client ID'),
    '#default_value' => $droogle_hangout_clientid,
    '#size' => 100,
    '#maxlength' => 150,
    '#description' => t('Enter the Google Client ID to use.  Visit https://cloud.google.com/console to generate a Client ID.'),
  );
  $form['droogle_hangout']['droogle_hangout_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Console Client Secret'),
    '#default_value' => $droogle_hangout_client_secret,
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the Google Client Secret to use.  Visit https://cloud.google.com/console to generate a Google Client Secret.'),
  );
  $form['droogle_hangout']['droogle_hangout_refresh_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Refresh Token'),
    '#default_value' => $droogle_hangout_refresh_token,
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the google refresh token created at https://developers.google.com/oauthplayground.'),
  );

  $form['droogle_hangout_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom CSS rules for Droogle Hangout'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['droogle_hangout_css']['droogle_hangout_custom_css'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom CSS for Hangout popup window'),
    '#default_value' => $droogle_hangout_custom_css,
    '#rows' => 10,
    '#description' => t('Add CSS rules as if this textarea were an actual CSS file'),
  );

  $form = system_settings_form($form);
  // Add to submit to save custom css to css/droogle_hangout_custom.css.
  $form['#submit'][] = 'droogle_hangout_write_css_file';

  return $form;
}

/**
 * Callback to write droogle_hangout_custom_css to custom css file.
 */
function droogle_hangout_write_css_file() {
  droogle_hangout_custom_css();
}

/**
 * Callback to write to css file.
 */
function droogle_hangout_custom_css() {
  global $base_url;
  // Hangout popup box.
  $default_css = '#content #block-droogle-hangout-droogle-hangout-invites h2,' . "\n";
  $default_css .= '#block-droogle-hangout-droogle-hangout-invites h2 {' . "\n";
  $default_css .= '  background-color: #629540;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '.slant-45 {' . "\n";
  $default_css .= '  border-right: 20px solid #629540;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '#hangout-invite-popup {' . "\n";
  $default_css .= '  border: solid 5px #629540;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '.hangout-btn-bubble {' . "\n";
  $default_css .= '  background-color: #9fc56d;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '.hangout-btn-bubble:after {' . "\n";
  $default_css .= '  border-right: 8px solid #9fc56d;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '#google-popup-logo {' . "\n";
  $default_css .= '  background: url(' . $base_url . '/sites/all/modules/custom/droogle_hangout/images/hangout_logo_green.png) no-repeat 0 0 transparent;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '#close-hangout-invite-popup:hover {' . "\n";
  $default_css .= '  background-color: #427520;' . "\n";
  $default_css .= '}' . "\n\n";
  // Hangout sent message box.
  $default_css .= '.hangout-sent {' . "\n";
  $default_css .= '  border: solid 3px #629540;' . "\n";
  $default_css .= '  border-top: solid 20px #629540;' . "\n";
  $default_css .= '}' . "\n\n";
  $default_css .= '.hangout-sent a:visited,' . "\n";
  $default_css .= '.hangout-sent a {' . "\n";
  $default_css .= '  background-color: #528530;' . "\n";
  $default_css .= '}' . "\n";

  $custom_css_file = drupal_get_path('module', 'droogle_hangout') . '/css/droogle_hangout_custom.css';
  if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
    $realpath = $wrapper->realpath();
    file_unmanaged_copy($custom_css_file, $realpath, FILE_EXISTS_REPLACE);
    $droogle_hangout_custom_css = check_plain(variable_get('droogle_hangout_custom_css', ''));
    file_put_contents($realpath . '/droogle_hangout_custom.css', $default_css . $droogle_hangout_custom_css);
  }
}
