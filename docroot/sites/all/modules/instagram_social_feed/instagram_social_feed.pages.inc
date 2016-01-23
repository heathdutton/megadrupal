<?php

/**
 * @file
 * Defines all hook menu callbacks.
 */

/**
 * Callback function for Instagram display table.
 */
function instagram_social_feed_overview($feed = NULL) {

  $api = variable_get('instagram_social_feed_api_key');
  if (!$api) {
    drupal_set_message(t('API Key not properly established. Please see the Manage Settings tab.'), 'error');
  }

  drupal_add_js(drupal_get_path('module', 'instagram_social_feed') . '/js/instagram_social_feed.js');

  $header = array(t('Thumbnail'), t('User'), t('Caption'), t('Timestamp'), t('Publish?'));

  $query = db_select('instagram_social_feed_photos', 's')
    ->fields('s', array())
    ->orderBy('instagram_id', 'DESC');
  $query = $query->extend('PagerDefault')->limit(100);

  $feed_id = arg(5);
  if (is_numeric($feed_id)) {
    $query->condition('feed_id', $feed_id, '=');
  }

  $results = $query->execute();
  $rows = array();
  while ($row = $results->fetch()) {

    $image_html = '<img src="' . $row->thumbnail . '" data-approved="' . $row->approve . '" />';

    $rows[] = array(
      l($image_html, $row->instagram_link, array(
        'html' => TRUE,
        'attributes' => array(
          'target' => '_blank'),
        )
      ),
      l($row->instagram_user, 'http://instagram.com/' . $row->instagram_user, array(
        'attributes' => array(
          'target' => '_blank'),
        )
      ),
      truncate_utf8(utf8_decode($row->caption), 80, FALSE, TRUE),
      date('Y-m-d g:i a', $row->created_time),
      $row->instagram_id,
    );
  }

  $variables = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => 'No photos yet',
  );

  $results = db_select('instagram_social_feeds', 's')
    ->fields('s', array())
    ->execute();

  $count = $results->rowCount();

  $feed_select = "";
  if ($count > 0) {
    $feed_select = "<select>";
    $feed_select .= '<option value="">All</option>';
    foreach ($results as $row) {
      $selected = ($feed == $row->id) ? 'selected' : '';
      $feed_select .= '<option value="' . $row->id . '" ' . $selected . '>' . $row->name . '</option>';
    }
    $feed_select .= "</select>";
  }

  return $feed_select . theme('table', $variables) . theme('pager');

}

/**
 * Callback function for settings form.
 */
function instagram_social_feed_settings() {

  $access_key = variable_get('instagram_social_feed_api_key', '');
  // Access token request in process.
  if (isset($_GET['code']) && $_GET['code'] != '') {
    if ($access_key == '') {
      $url = "https://api.instagram.com/oauth/access_token";
      $fields = array(
        "client_id" => variable_get("instagram_social_feed_client_id"),
        "client_secret" => variable_get("instagram_social_feed_client_secret"),
        "grant_type" => "authorization_code",
        "redirect_uri" => variable_get("instagram_social_feed_redirect_uri"),
        "code" => $_GET['code'],
      );

      $fields_string = '';
      foreach ($fields as $key => $value) {
        $fields_string .= $key . '=' . $value . '&';
      }
      rtrim($fields_string, '&');

      // Request access token.
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_POST, count($fields));
      curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);

      $output = curl_exec($ch);
      curl_close($ch);

      $auth = json_decode($output);

      if (empty($auth->error_message)) {
        variable_set('instagram_social_feed_api_key', $auth->access_token);
        variable_set('instagram_social_feed_user_id', $auth->user->id);
        variable_set('instagram_social_feed_username', $auth->user->username);

        $access_key = $auth->access_token;
        drupal_set_message(t('Instagram authentication successful'));
      }
      else {
        drupal_set_message($auth->error_message, 'error');
      }
    }
  }
  elseif (array_key_exists('code', $_GET) && $_GET['code'] == '') {

      // Remove api key for re-authentication.
      variable_del('instagram_social_feed_api_key');
      // Unset variable for form.
      $access_key = '';
  }

  $form = array();
  if ($access_key == '') {

    // Non-authenticated settings form.
    $form['instagram_social_feed_client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram Client ID'),
      '#default_value' => variable_get('instagram_social_feed_client_id', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => t('You must register an Instagram client key to use this module. You can register a client by <a href="http://instagram.com/developer/clients/manage/" target="_blank">clicking here</a>.'),
    );
    $form['instagram_social_feed_client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram Client Secret'),
      '#default_value' => variable_get('instagram_social_feed_client_secret', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => t('The client secret can be found after creating an Instagram client in the API console.'),
    );
    $form['instagram_social_feed_redirect_uri'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram Redirect URI'),
      '#default_value' => variable_get('instagram_social_feed_redirect_uri', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#description' => t('Set the redirect URI to :url', array(
        ':url' => url('admin/config/services/instagram_social_feed/settings'),
      )),
    );

    if (variable_get('instagram_social_feed_client_id', '') != '' && variable_get('instagram_social_feed_redirect_uri', '') != '') {
      $form['authenticate'] = array(
        '#markup' => l(t('Click here to authenticate via Instagram and create an access token'),
          'https://api.instagram.com/oauth/authorize/?client_id=' . variable_get('instagram_social_feed_client_id') . '&redirect_uri=' . variable_get('instagram_social_feed_redirect_uri') . '&response_type=code'
        )
      );
    }
  }
  else {

    // Authenticated user settings form.
    $form['instagram_social_feed_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Instagram API Key'),
      '#default_value' => variable_get('instagram_social_feed_api_key', ''),
      '#size' => 60,
      '#maxlength' => 255,
      '#disabled' => TRUE,
      '#description' => t('Stored access key for accessing the API key'),
    );
    $form['authenticate'] = array(
      '#markup' => l(t('Click here to remove the access key and re-authenticate via Instagram'),
        'admin/config/services/instagram_social_feed/settings/', array(
          'query' => array('code' => ''),
        )
      ),
    );
  }

  return system_settings_form($form);

}

/**
 * Callback function for module info page.
 */
function instagram_social_feed_status_form() {

  $api = variable_get('instagram_social_feed_api_key');
  if (!$api) {
    drupal_set_message(t('API Key not properly established. Please see the Manage Settings tab.'), 'error');
  }

  $line1 = t('This module is connected to the Instagram account: %account', array('%account' => variable_get('instagram_social_feed_username', '')));
  $message = array(
    t('This module is connected to the Instagram account: %account', array('%account' => variable_get('instagram_social_feed_username', ''))),
    '<br /><br />',
    t('New photos will be requested on each cron run. Make sure cron is configured to run regularly.'),
    '<br /><br />',
    t('To pull new photos manually, click %run below:', array('%run' => t('Run'))),
    '<br /><br />',
  );

  $form = array();
  $form['description'] = array(
    '#markup' => implode('', $message),
  );

  $form['run'] = array(
    '#type' => 'submit',
    '#value' => t('Run'),
    '#submit' => array('instagram_social_feed_status_form_submit'),
  );

  $results = db_select('instagram_social_feed_photos', 's')->fields('s')->execute();
  $count = $results->rowCount();

  if ($count > 0) {
    $form['delete'] = array(
      '#prefix' => '<p>',
      '#markup' => l(t('Click here to delete all photos.'), 'admin/config/services/instagram_social_feed/delete'),
      '#suffix' => '</p>',
    );
  }

  return $form;

}

/**
 * Callback function for manually running an API request.
 */
function instagram_social_feed_status_form_submit($form, &$form_state) {
  instagram_social_feed_cron();
}

/**
 * AJAX callback for approving individual photos.
 */
function instagram_social_feed_approve() {

  if (isset($_GET['instagram_id'])) {
    $id = $_GET['instagram_id'];
  }
  else {
    header("HTTP/1.1 500 Internal Server Error");
    exit();
  }

  $result = db_update('instagram_social_feed_photos')
    ->expression('approve', 'IF(approve=1, 0, 1)')
    ->condition('instagram_id', $id)
    ->execute();

}

/**
 * Delete all photos confirmation.
 */
function instagram_social_feed_delete_confirm($form, &$form_state) {
  $form = array();
  return confirm_form($form,
    t('Are you sure you want to delete all photos?'),
    'admin/config/services/instagram_social_feed',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete all photos confirm submit.
 */
function instagram_social_feed_delete_confirm_submit($form, &$form_state) {
  $form_values = $form_state['values'];

  if ($form_values['confirm']) {
    $num_deleted = db_delete('instagram_social_feed_photos')
      ->execute();
    drupal_set_message(t('Successfully deleted :num photos.', array(':num' => $num_deleted,)));
  }

  drupal_goto('admin/config/services/instagram_social_feed');
}
