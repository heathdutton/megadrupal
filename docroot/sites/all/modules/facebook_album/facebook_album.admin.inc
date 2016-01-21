<?php
/**
 * @file facebook_album.admin.inc
 * Facebook Album administration pages.
 */



/**
 * Implements hook_admin().
 *
 */
function facebook_album_admin_form() {
  $form = array();

  $form = system_settings_form($form);

  $form['facebook_album_pageID'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Page ID'),
    '#default_value' => variable_get('facebook_album_pageID', ''),
    '#description' => t('The page ID of the page you want to pull the albums from. For example, if your page is https://facebook.com/acromediainc, you would enter acromediainc.'),
  );

  $form['facebook_album_api_version'] = array(
    '#type' => 'select',
    '#options' => array(
      'v2.3' => '2.3',
      'v2.4' => '2.4',
    ),
    '#title' => t('Facebook API Version'),
    '#default_value' => variable_get('facebook_album_api_version', 'v2.3'),
    '#description' => t("The application API version, currently only 2.3 and 2.4 are supported."),
  );

  $form['facebook_album_appID'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Application ID'),
    '#default_value' => variable_get('facebook_album_appID', ''),
    '#description' => t("The application ID specified in your Facebook App's dashboard page."),
  );

  $form['facebook_album_appSecret'] = array(
    '#type' => 'password',
    '#title' => t('Facebook Application Secret'),
    '#default_value' => '',
    '#description' => t("The application secret specified in your Facebook App's dashboard page. This field remains blank for security purposes. If you have already saved your application secret, leave this field blank, unless you wish to update it."),
  );

  $form['facebook_album_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Album Visibility'),
    '#options' => array(
      0 => t('Exclude the listed albums'),
      1 => t('Only show the specified albums'),
    ),
    '#default_value' => variable_get('facebook_album_visibility', 0),
  );

  $form['facebook_album_albums'] = array(
    '#type' => 'textarea',
    '#description' => t('Leave blank to show all albums. Specify albums by using their album IDs. Enter one ID per line.'),
    '#default_value' => implode("\n", variable_get('facebook_album_albums', array())),
  );

  $form['display_settings'] = array(
    '#title' => t('Album Display Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['display_settings']['facebook_album_albumLimit'] = array(
    '#title' => t('Album Limit'),
    '#type' => 'textfield',
    '#default_value' => variable_get('facebook_album_albumLimit', 3),
    '#description' => t('Leave blank or set to 0 if you want to load all albums'),
  );

  $form['display_settings']['facebook_album_showDescription'] = array(
    '#title' => t('Show Album Description'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('facebook_album_showDescription', 1),
  );

  $form['display_settings']['facebook_album_showLocation'] = array(
    '#title' => t('Show Album Location'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('facebook_album_showLocation', 1),
  );

  $form['display_settings']['facebook_album_albumThumbWidth'] = array(
    '#title' => t('Album Thumbnail Width'),
    '#type' => 'textfield',
    '#default_value' => variable_get('facebook_album_albumThumbWidth', 365),
  );

  $form['display_settings']['facebook_album_albumThumbHeight'] = array(
    '#title' => t('Album Thumbnail Height'),
    '#type' => 'textfield',
    '#default_value' => variable_get('facebook_album_albumThumbHeight', 250),
  );

  $form['display_settings']['facebook_album_photoThumbWidth'] = array(
    '#title' => t('Photo Thumbnail Width'),
    '#type' => 'textfield',
    '#default_value' => variable_get('facebook_album_photoThumbWidth', 160),
  );

  $form['display_settings']['facebook_album_photoThumbHeight'] = array(
    '#title' => t('Photo Thumbnail Height'),
    '#type' => 'textfield',
    '#default_value' => variable_get('facebook_album_photoThumbHeight', 120),
  );

  $form['display_settings']['colorbox'] = array(
    '#title' => t('Colorbox Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Facebook Albums support displaying images in Colorbox if it is enabled.'),
  );

  $form['display_settings']['colorbox']['facebook_album_colorboxOptions'] = array(
    '#title' => t('Colorbox Options'),
    '#type' => 'textarea',
    '#default_value' => variable_get('facebook_album_colorboxOptions', ''),
    '#description' => t('Specify any additional Colorbox options here. i.e. "transition:\'elastic\', speed:350"'),
  );

  array_unshift($form['#submit'], 'facebook_album_admin_form_submit');

  return $form;
}

/**
 * Implements hook_admin()
 *
 * Attempts to fetch an application access token from facebook
 * based off of the app ID and app secret specified in the form
 */
function facebook_album_admin_form_submit($form, &$form_state) {

  $form_state['values']['facebook_album_albums'] = explode("\r\n", $form_state['values']['facebook_album_albums']);
  variable_set('facebook_album_access_token', null);

  $app_id = $form_state['values']['facebook_album_appID'];
  $app_secret = $form_state['values']['facebook_album_appSecret'];

  if (empty($app_secret) || strlen(trim($app_secret)) < 1) {
    $app_secret = variable_get('facebook_album_appSecret');
    unset($form_state['values']['facebook_album_appSecret']);
  }

  $token = _facebook_album_fetch_application_access_token($app_id, $app_secret);

  if (isset($token['error'])) {
    $message = _facebook_album_translate_api_error($token['error']['code'], $token['error']['message']);
    drupal_set_message($message, 'error');
  }
  else {
    variable_set('facebook_album_access_token', $token['access_token']);
  }
}


/**
 * Implements hook_validate()
 */
function facebook_album_admin_form_validate($form, &$form_state) {

  $app_id = $form_state['values']['facebook_album_appID'];
  $app_secret = $form_state['values']['facebook_album_appSecret'];

  $stored_app_secret = variable_get('facebook_album_appSecret');

  if (empty($stored_app_secret) && empty($app_secret)) {
    form_set_error('facebook_album_appSecret', t("Missing Application Secret"));
  }
}


/**
 * Build and make the api call for retrieving application access tokens
 * from the facebook api.
 *
 * @param $app_id
 *    The application ID specified in the settings menu
 * @param $app_secret
 *    The application secret specified in the settings menu
 * @return mixed
 *    A response rendered as an array
 */
function _facebook_album_fetch_application_access_token( $app_id, $app_secret ) {

  $url = _facebook_album_build_api_request(API_AUTH_PATH . 'access_token', array(
      'client_id'     => $app_id,
      'client_secret' => $app_secret,
      'grant_type'    => 'client_credentials',
    )
  );

  return _facebook_album_fetch_api_response($url);
}
