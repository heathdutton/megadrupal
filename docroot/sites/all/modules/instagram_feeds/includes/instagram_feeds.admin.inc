<?php

/**
 * @file
 * Administration form with Instagram settings.
 */

/**
 * Form constructor for the Instagram Feeds settings form.
 */
function instagram_feeds_settings($form, &$form_state) {
  global $base_url;

  $form['instagram_feeds_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Client ID'),
    '#default_value' => variable_get('instagram_feeds_client_id', ''),
    '#description' => t('A client ID can be received !link. You must have a registered client.', array(
      '!link' => l(t('here'), 'http://instagram.com/developer/clients/manage'),
    )),
  );
  $form['instagram_feeds_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Client Secret'),
    '#default_value' => variable_get('instagram_feeds_client_secret', ''),
    '#description' => t('Obtained with Client ID.'),
  );
  $form['instagram_feeds_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Access Token'),
    '#default_value' => variable_get('instagram_feeds_access_token', ''),
    '#description' => t('To receive an Access Token, click !link and authorize then.', array(
      '!link' => l(t('here'), 'https://instagram.com/oauth/authorize/?redirect_uri='
          . $base_url . urlencode('?q=' . $_GET['q']) . '&response_type=code'
          ),
    )),
    '#states' => array(
      'visible' => array(
        ':input[name="instagram_feeds_client_id"]' => array('!value' => ''),
        ':input[name="instagram_feeds_client_secret"]' => array('!value' => ''),
      ),
    ),
  );
  if (isset($_GET['access_token'])) {
    $form['instagram_feeds_access_token']['#default_value'] = $_GET['access_token'];
    variable_set('instagram_feeds_access_token', $_GET['access_token']);
    drupal_set_message(t('Access token received successfully.'));
  }
  $form['instagram_feeds_empty_client_id'] = array(
    '#type' => 'item',
    '#title' => t('Instagram Access Token'),
    '#markup' => t('To receive an access token you must enter the Client ID and Client Secret first.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="instagram_feeds_client_id"]' => array('!value' => ''),
        ':input[name="instagram_feeds_client_secret"]' => array('!value' => ''),
      ),
    ),
  );
  if (isset($_GET['code']) && !empty($_GET['code'])) {
    // Getting response from Instagram server for receiving access token.
    $post_data = array(
      'client_id=' . variable_get('instagram_feeds_client_id'),
      'client_secret=' . variable_get('instagram_feeds_client_secret'),
      'grant_type=authorization_code',
      'redirect_uri=' . $base_url . urlencode('?q=' . $_GET['q']),
      'code=' . $_GET['code'],
    );
    $options = array(
      'method' => 'POST',
      'data' => implode('&', $post_data),
    );
    $response = drupal_http_request('https://api.instagram.com/oauth/access_token', $options);
    if ('OK' == $response->status_message) {
      $data = drupal_json_decode($response->data);
      variable_set('instagram_feeds_access_token', $data['access_token']);
      $form['instagram_feeds_access_token']['#default_value'] = variable_get('instagram_feeds_access_token');
      drupal_set_message(t('An Access Token successfully generated.'));
    }
    else {
      drupal_set_message(t('Something wrong. Invalid response. Please check Client ID and Client secret values.'),
          'error');
    }
    drupal_goto('admin/config/services/instagram');
  }
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'instagram_feeds') . '/js/access-token-gen.js',
  );

  $form['custom_settings'] = array(
    '#type' => 'fieldset',
  );
  $expiration_time = variable_get('instagram_feeds_items_expiration_time', 2592000);
  $options = array(86400, 604800, 1209600, 2592000, 5184000);
  $form['custom_settings']['instagram_feeds_items_expiration_time'] = array(
    '#type' => 'select',
    '#title' => t('Items Expiration Time'),
    '#description' => t('Delete Instagram Items older than selected time.'),
    '#options' => array(0 => t('Never')) + drupal_map_assoc($options, 'format_interval'),
    '#default_value' => $expiration_time,
  );
  $options = array(30, 40, 50, 60, 100, 150, 200);
  $limit_imgs_per_feed = variable_get('instagram_feeds_limit_imgs_per_feed', 0);
  $form['custom_settings']['instagram_feeds_limit_imgs_per_feed'] = array(
    '#type' => 'select',
    '#title' => t('Max number of Images per Hash Tag'),
    '#description' => t('Automaticly remove older images if this amount is reached (for each hashtag used). This setting DOES NOT cancel expiration time setting.'),
    '#options' => array(0 => t('Unlimited')) + drupal_map_assoc($options),
    '#default_value' => $limit_imgs_per_feed,
  );
  $remove_unused_terms = variable_get('instagram_feeds_remove_unused_terms',
        min(array(1, max($expiration_time, $limit_imgs_per_feed))));
  $form['custom_settings']['instagram_feeds_remove_unused_terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete Unused Terms'),
    '#description' => t('Remove unused users that are not in blacklist, and unused hash tags. If checked, unused Instagram users and hashtags will be also deleted while deleting old Instagram Items. (Recommeded: ON)'),
    '#return_value' => 1,
    '#default_value' => $remove_unused_terms,
    '#states' => array(
      'invisible' => array(
        '#edit-instagram-feeds-items-expiration-time' => array('value' => '0'),
        '#edit-instagram-feeds-limit-imgs-per-feed' => array('value' => '0'),
      ),
    ),
  );
  $options = drupal_map_assoc(array(1, 2, 3));
  $form['custom_settings']['instagram_feeds_download_attempts'] = array(
    '#type' => 'select',
    '#title' => t('Number of Attempts to Download an Image'),
    '#description' => t('If the image from Instagram can not be downloaded by different reasons, an attempt to download will be repeated the selected number of times.'),
    '#options' => array(0 => t('Do not attempt to download')) + $options,
    '#default_value' => variable_get('instagram_feeds_download_attempts', 0),
  );
  $form = system_settings_form($form);
  $form['#submit'][] = 'instagram_feeds_settings_submit';
  return $form;
}


/**
 * Submit handler for instagram_feeds_import_tab_form().
 */
function instagram_feeds_settings_submit($form, &$form_state) {
  global $base_url;
  if (empty($form_state['values']['instagram_feeds_access_token'])
      && !empty($form_state['values']['instagram_feeds_client_secret'])
      && !empty($form_state['values']['instagram_feeds_client_id'])) {
    $path = 'https://instagram.com/oauth/authorize/?redirect_uri=' . $base_url
        . urlencode('?q=' . 'admin/config/services/instagram') . '&response_type=code' . '&client_id='
        . $form_state['values']['instagram_feeds_client_id'];
    drupal_goto($path);
  }
}
