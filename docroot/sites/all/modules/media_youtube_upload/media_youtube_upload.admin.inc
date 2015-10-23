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
  $form['#id'] = 'media-youtube-upload-account-settings-form';

  $form['google_app'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Google Application'),
  );
  // Get the token if we have one.
  $token = variable_get('media_youtube_upload_token', array());
  // Display just the application information if we have a token present.
  if (!empty($token['refresh_token'])) {
    $form['google_app']['#description'] = t('Google application is succesfully authenticated.');
    $form['google_app']['media_youtube_upload_app_name'] = array(
      '#type' => 'item',
      '#title' => t('Application name') . ': ',
      '#markup' => variable_get('media_youtube_upload_app_name', ''),
    );
    $form['google_app']['media_youtube_upload_client_id'] = array(
      '#type' => 'item',
      '#title' => t('Client ID') . ': ',
      '#markup' => variable_get('media_youtube_upload_client_id', ''),
    );
    $form['google_app']['media_youtube_upload_client_secret'] = array(
      '#type' => 'item',
      '#title' => t('Client secret') . ': ',
      '#markup' => variable_get('media_youtube_upload_client_secret', ''),
    );
    $form['#attributes']['class'] = array('successfully-authenticated');

    $form['#attached']['css'] = array(
      drupal_get_path('module', 'media_youtube_upload') . '/css/media_youtube_upload.css',
    );

    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset application'),
      '#weight' => 2,
    );
  }
  // If no token is present, display the application form.
  else {
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
    global $base_url;
    $form['google_app']['media_youtube_upload_client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Client secret'),
      '#description' => '<em>xxx..apps.googleusercontent.com</em><br />' . t('Get credentials keys on !here <br />Next in "APIs & auth" -> "Credentials" -> "OAuth". Click "Create new Client ID" and then under "Application Type" select "Web Application".<br />For the redirect uri use <b>!base_url/media-youtube-upload/oauth2callback</b>',
        array(
          '!here' => l(t('https://console.developers.google.com'), 'https://console.developers.google.com/project'),
          '!base_url' => $base_url,
        )),
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
  }

  $form['google_app']['media_youtube_upload_automatic_deletion'] = array(
    '#type' => 'checkbox',
    '#title' => '<b>' . t('Automatic deletion') . '</b>',
    '#description' => t('If this checkbox is checked all YouTube videos deleted on this site will also get deleted on YouTube. Be carefull with this option. <b>If you wish to keep your videos online do not check this box!</b> You can also use the action "Delete locally and remotely" if you wish to have more granular control.'),
    '#default_value' => variable_get('media_youtube_upload_automatic_deletion', FALSE),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 1,
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
  if ($form_state['triggering_element']['#id'] === 'edit-submit') {
    if (isset($vals['media_youtube_upload_app_name']) && isset($vals['media_youtube_upload_client_secret']) && isset($vals['media_youtube_upload_client_id'])) {

      // If account has changed, the autorisation should be done again.
      if (variable_get('media_youtube_upload_app_name', '') != $vals['media_youtube_upload_app_name'] ||
          variable_get('media_youtube_upload_client_secret', '') != $vals['media_youtube_upload_client_secret'] ||
          variable_get('media_youtube_upload_client_id', '') != $vals['media_youtube_upload_client_id']
          ) {
        variable_del('media_youtube_upload_token');
      }

      if (empty(variable_get('media_youtube_upload_token', ''))) {
        $ytapi = new MediaYoutubeUploadYtapi();
        $redirect = $ytapi->getAuthUrl();
        $form_state['redirect'] = $redirect;
      }
    }
    form_state_values_clean($form_state);
    // Populate variables.
    foreach ($form_state['values'] as $key => $val) {
      // Convert settings to variables.
      variable_set($key, trim($val));
    }
    drupal_set_message(t('Configuration successfully saved.'), 'status');
  }
  elseif ($form_state['triggering_element']['#id'] === 'edit-reset') {
    variable_del('media_youtube_upload_token');
    drupal_set_message(t('The access token has been deleted. Please re-save the application with the right credentials.'), 'warning');
  }
}
