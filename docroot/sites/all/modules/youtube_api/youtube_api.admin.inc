<?php

/**
 * @file
 * Admin forms and callbacks for youtube api module.
 */

/**
 * General settings form.
 */
function youtube_api_settings_form($form, &$form_state) {
  $form['web_application'] = array(
    '#type' => 'fieldset',
    '#title' => t('Web application setup'),
    '#description' => t('These credentials are only for web application use.'),
    '#collapsible' => TRUE,
  );

  $form['web_application']['youtube_api_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client id'),
    '#description' => t('Enter the client id for the Google OAuth 2 credientials.'),
    '#default_value' => variable_get('youtube_api_client_id', ''),
  );

  $form['web_application']['youtube_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#description' => t('Enter the secret key associated with the client id.'),
    '#default_value' => variable_get('youtube_api_secret', ''),
  );

  return system_settings_form($form);
}

/**
 * Menu callback: Google redirect location - store token.
 */
function youtube_api_store_token_page() {
  if ($_GET['code']) {
    $client = youtube_api_get_google_client();
    if ($client) {
      $client->setRedirectUri(url('youtube-api/store-token', array('absolute' => TRUE)));
      $client->authenticate($_GET['code']);
      variable_set('youtube_api_access_token', $client->getAccessToken());
    }
  }
  if ($redirect = variable_get('youtube_api_redirect_location', '')) {
    variable_del('youtube_api_redirect_location');
    return drupal_goto($redirect);
  }
}
