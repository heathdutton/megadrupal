<?php
/**
 * @file
 * This file holds functions for the sn_fb_dynamic_cover_photo Admin settings.
 *
 * @SNDCP sn_fb_dynamic_cover_photo
 */

/**
 * Menu callback; displays the sn_fb_dynamic_cover_photo module settings page.
 *
 * @see system_settings_form()
 */
function sn_fb_dynamic_cover_photo_admin_settings_form($node, &$form_state) {
  $access_token = sn_fb_dynamic_cover_photo_get_access_token_from_db();
  $app_id = variable_get('sn_fb_dynamic_cover_photo_app_id');
  $redirect_uri = variable_get('sn_fb_dynamic_cover_photo_redirect_uri');
  $secret = variable_get('sn_fb_dynamic_cover_photo_secret');

  if (!empty($app_id) && !empty($redirect_uri) && !empty($secret) && empty($access_token)) {
    sn_fb_dynamic_cover_photo_create_access_token($access_token, array(
        'app_id' => $app_id,
        'redirect_uri' => $redirect_uri,
        'secret' => $secret));
  }

  // Get the list of options to populate the first dropdown.
  $options_first = sn_fb_dynamic_cover_photo_first_dropdown_options();
  $value_dropdown_first = isset($form_state['values']['sn_node_type']) ? $form_state['values']['sn_node_type'] : key($options_first);

  $form = array();

  $form['sn_fb_dynamic_cover_photo_brand_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Brand Page'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_brand_page', ''),
    '#required' => TRUE,
    '#description' => t('Facebook brand page name where cover photo needs to be changed'),
    '#attributes' => array('placeholder' => 'YOUR BRAND PAGE'),
  );

  $form['sn_fb_dynamic_cover_photo_album_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cover Album Name'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_album_name', 'DCP'),
    '#required' => TRUE,
    '#attributes' => array('readonly' => 'readonly'),
    '#description' => t('Facebook Album Name'),
  );

  $form['sn_fb_dynamic_cover_photo_upload_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload Directory'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_upload_directory', 'sites/all/files/DCP/'),
    '#required' => TRUE,
    '#description' => t('Upload Directory Name where photo needs to be uploaded'),
  );

  $form['sn_fb_dynamic_cover_photo_redirect_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_redirect_uri', 'admin/config/system/sn-dcp'),
    '#required' => TRUE,
    '#attributes' => array('readonly' => 'readonly'),
    '#description' => t('Redirect URL (Used in access token generation from facebook)'),
  );

  $form['sn_fb_dynamic_cover_photo_scope'] = array(
    '#type' => 'textarea',
    '#title' => t('Scope'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_scope', SN_FB_DYNAMIC_COVER_PHOTO_SCOPE),
    '#required' => TRUE,
    '#attributes' => array('readonly' => 'readonly'),
    '#description' => t('Facebook permissions'),
  );

  $form['sn_fb_dynamic_cover_photo_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App Id'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_app_id', ''),
    '#required' => TRUE,
    '#description' => t('Facebook App Id'),
    '#attributes' => array('placeholder' => 'YOUR FACEBOOK APP ID'),
  );

  $form['sn_fb_dynamic_cover_photo_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#default_value' => variable_get('sn_fb_dynamic_cover_photo_secret', ''),
    '#required' => TRUE,
    '#description' => t('Facebook App Secret Key'),
    '#attributes' => array('placeholder' => 'YOUR FACEBOOK APP SECRET KEY'),
  );

  $form['sn_node_type'] = array(
    '#type' => 'select',
    '#title' => t('Node Type'),
    '#options' => $options_first,
    '#default_value' => $value_dropdown_first,
    '#ajax' => array(
      'callback' => 'sn_fb_dynamic_cover_photo_ajax_callback',
      'wrapper' => 'dropdown_second_replace',
    ),
    '#required' => TRUE,
    '#description' => t('Node type list'),
  );

  $form['sn_image_field'] = array(
    '#type' => 'select',
    '#title' => t('Field Name'),
    '#prefix' => '<div id="dropdown_second_replace">',
    '#suffix' => '</div>',
    '#options' => sn_fb_dynamic_cover_photo_second_dropdown_options($value_dropdown_first),
    '#description' => t('Image Field from selected node type'),
  );

  // Make a system setting form and return.
  return system_settings_form($form);
}

/**
 * Function for ajax call back for second dropdown.
 *
 * @return array
 *   Array of options for sceond dropdown
 */
function sn_fb_dynamic_cover_photo_ajax_callback($form, $form_state) {
  return $form['sn_image_field'];
}

/**
 * Function for node type options.
 *
 * @return array
 *   Array of options for first dropdown
 */
function sn_fb_dynamic_cover_photo_first_dropdown_options() {
  $node_type_all = field_info_instances("node");
  foreach ($node_type_all as $key => $value) {
    $node_type[$key] = $key;
  }

  return $node_type;
}

/**
 * Function to get list of fields with image upload field from any content type.
 *
 * @param string $key
 *   String of node type.
 *
 * @return array
 *   Array option for second dropdown.
 */
function sn_fb_dynamic_cover_photo_second_dropdown_options($key = '') {
  $content_type = field_info_instances("node", $key);
  $options = array();
  foreach ($content_type as $k => $field) {
    if (isset($field['settings']['file_directory']) && $field['settings']['file_directory'] == 'field/image') {
      $options[$k] = $k;
    }
  }
  if (isset($options) && !empty($options)) {
    return $options;
  }
  else {
    return array();
  }
}

/**
 * Function to create Access Token.
 *
 * @global string $base_url
 *
 * @param string $access_token
 *   Access token value from database.
 * @param array $fb_data
 *   Data for app id, redirect uri and secret key in array format.
 */
function sn_fb_dynamic_cover_photo_create_access_token($access_token, $fb_data) {
  global $base_url;
  $scope = variable_get('sn_fb_dynamic_cover_photo_scope');
  $app_id = $fb_data['app_id'];
  $redirect_uri = $fb_data['redirect_uri'];
  $secret = $fb_data['secret'];
  // Include facebook files.
  sn_fb_dynamic_cover_photo_include_fb_files();

  $facebook = new Facebook(array(
    'appId' => variable_get('sn_fb_dynamic_cover_photo_app_id', $app_id),
    'secret' => variable_get('sn_fb_dynamic_cover_photo_secret', $secret))
  );

  if (empty($access_token)) {
    $code = isset($_REQUEST["code"]) ? $_REQUEST["code"] : '';

    if (empty($code)) {
      $_SESSION['state'] = md5(uniqid(rand(), TRUE));
      $dialog_url = "https://www.facebook.com/dialog/oauth?client_id="
      . $app_id . "&redirect_uri=" . $base_url . '/' . $redirect_uri . '/' . "&state=" . $_SESSION['state'] . "&scope=" . $scope;
      echo "<script>top.location.href='" . $dialog_url . "'</script>";
    }

    $token_url = "https://graph.facebook.com/oauth/access_token?client_id=$app_id&redirect_uri=$base_url/$redirect_uri/&client_secret=$secret&code=$code";
    // Curl request to push image as photo album.
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $token_url);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

    $result = curl_exec($ch);
    curl_close($ch);
    $error_message = json_decode($result);
    if (isset($error_message->error) && count($error_message->error) > 0) {
      watchdog('sn_fb_dynamic_cover_photo_token', $error_message->error->message, array(), WATCHDOG_NOTICE);
    }
    else {
      $access_token = explode('access_token=', $result);
      $access_token = explode('&expires=', $access_token[1]);
      $access_token = $access_token[0];
      $expiry = mktime(0, 0, 0, date('m') + 2, date('d'), date('Y'));
      // Inserting mysql entry to update/store access token.
      if (!empty($access_token)) {
        $result = db_truncate('sn_fb_dcp_config')->execute();
        $result_insert = db_insert('sn_fb_dcp_config')
                ->fields(array('access_token', 'expires'))
                ->values(array(
                    'access_token' => $access_token,
                    'expires' => $expiry,
                    ))
                ->execute();

        if (isset($result_insert)) {
          watchdog('sn_fb_dynamic_cover_photo_token', 'Access Token Generated Successfully', array(), WATCHDOG_NOTICE);
          $facebook->setAccessToken($access_token);
        }
      }
    }
  }
  else {
    watchdog('sn_fb_dynamic_cover_photo_token', "Access Token Not Generated", array(), WATCHDOG_NOTICE);
  }
}
