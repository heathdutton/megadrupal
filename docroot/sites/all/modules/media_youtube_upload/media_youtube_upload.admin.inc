<?php
/**
 * @file
 * Administrative page callbacks for the youtube_upload module.
 */

/**
 * General configuration form for youtube (account info + youtube settings).
 */
function media_youtube_upload_account_settings_form($form, $form_state) {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  // Test if we get the google API Library client.
  $ytapi = new MediaYoutubeUploadYtapi();
  if (!$ytapi->mtestGoogleLib()) {
    $form['lib_error']['#markup'] = t('Google Libraries not found.<br />Use drush to download them <em>drush ytu-libraries</em><br /> or check the !status for the manual procedure, section Google API and Google CORS.', array(
          '!status' => l(t('Status report'), 'admin/reports/status'),
      ));
    return $form;
  }

  $token = variable_get('media_youtube_upload_token', array());

  // Display the authorization link.
  if (empty($token['refresh_token']) && variable_get('media_youtube_upload_app_name', '') != '') {
    // Test the connection to google.
    $ytapi = new MediaYoutubeUploadYtapi();
    $res = $ytapi->getAuthUrl();
    drupal_set_message(t('You need to allow your application by following !link_auth', array('!link_auth' => l(t('this link'), check_url($res)))), 'warning', FALSE);
  }
  elseif (!empty($token['refresh_token'])) {
    drupal_set_message(t('Token acquired from Google'), 'status', FALSE);
  }

  $form['google_app'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Google Application'),
  );
  $form['google_app']['media_youtube_upload_app_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application name'),
    '#default_value' => variable_get('media_youtube_upload_app_name', ''),
    '#required' => TRUE,
  );
  $form['google_app']['media_youtube_upload_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('media_youtube_upload_client_id', ''),
    '#required' => TRUE,
  );
  $protocol = (isset($_SERVER['HTTPS'])) ? 'https' : 'http';
  $form['google_app']['media_youtube_upload_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#description' => '<em>xxx..apps.googleusercontent.com</em><br />' . t('Get credentials keys on !here <br />Next in "APIs & auth" -> "Credentials" -> "OAuth". Click "Create new Client ID" and then under "Application Type" select "Web Application".<br />For the redirect uri use !protocol :// !server_name /media_youtube_upload/oauth2callback',
      array(
        '!here' => l(t('https://console.developers.google.com'), 'https://console.developers.google.com/project'),
        '!protocol' => $protocol,
        '!server_name' => $_SERVER['SERVER_NAME'])),
    '#default_value' => variable_get('media_youtube_upload_client_secret', ''),
    '#required' => TRUE,
  );
  $country = _media_youtube_upload_get_country();
  $countries = country_get_list();
  $form['google_app']['media_youtube_upload_app_country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#description' => t('The country is required to get video category list. If you change this setting after you have data in the category field it is possible you will lose data.'),
    '#options' => $countries,
    '#default_value' => $country,
    '#required' => TRUE,
    '#prefix' => l('', '', array('attributes' => array('name' => 'media-youtube-upload-app-country'))),
  );

  $form['google_app']['media_youtube_upload_automatic_deletion'] = array(
    '#type' => 'checkbox',
    '#title' => '<b>' . t('Automatic deletion') . '</b>',
    '#description' => t('If this checkbox is checked all YouTube videos deleted on this site will also get deleted on YouTube. Be carefull with this option. <b>If you wish to keep your videos online do not check this box!</b> You can also use the action "Delete locally and remotely" if you wish to have more granular control.'),
    '#default_value' => variable_get('media_youtube_upload_automatic_deletion', FALSE),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('save configuration'),
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * @see media_youtube_upload_account_settings_form()
 */
function media_youtube_upload_account_settings_form_submit($form, &$form_state) {

  $vals = $form_state['values'];

  // If account has changed, the autorisation should be done again.
  if (variable_get('media_youtube_upload_app_name', '') != $vals['media_youtube_upload_app_name'] ||
      variable_get('media_youtube_upload_client_secret', '') != $vals['media_youtube_upload_client_secret'] ||
      variable_get('media_youtube_upload_client_id', '') != $vals['media_youtube_upload_client_id']
      ) {
    variable_del('media_youtube_upload_token');
  }

  form_state_values_clean($form_state);

  // Populate variables.
  foreach ($form_state['values'] as $key => $val) {
    // Convert settings to variables.
    variable_set($key, $val);
  }
}
