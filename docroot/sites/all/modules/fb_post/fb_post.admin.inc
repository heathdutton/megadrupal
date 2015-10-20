<?php

/**
 * @file
 * Settings.
 */

/******************************* General Settings *****************************/

/**
 * FB app settings form.
 */
function fb_post_app_settings_form($form, &$form_state) {
  $app_id = variable_get('fb_post_app_id', '');
  $app_secret = variable_get('fb_post_app_secret', '');
  $app_access_token = variable_get('fb_post_app_access_token', '');

  $form['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#description' => t('Your Facebook application ID.'),
    '#default_value' => $app_id,
    '#required' => TRUE,
  );
  $form['app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#description' => t('Your Facebook application secret.'),
    '#default_value' => $app_secret,
    '#required' => TRUE,
  );
  if ($app_access_token) {
    $form['app_access_token'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . t('App access token: ') . $app_access_token . '</p>',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate handler for fb_post_app_settings_form.
 */
function fb_post_app_settings_form_validate($form, &$form_state) {
  module_load_include('inc', 'fb_post');

  $values = $form_state['values'];
  $app_access_token = fb_post_get_app_access_token($values['app_id'], $values['app_secret']);
  if ($app_access_token) {
    variable_set('fb_post_app_access_token', $app_access_token);
  }
  else {
    drupal_set_message(t('Incorrect a Facebook App access token. Please check the Facebook App ID and the App key.'), 'error');
    form_set_error('app_id');
    form_set_error('app_secret');
  }
}

/**
 * Submit handler for fb_post_app_settings_form.
 */
function fb_post_app_settings_form_submit($form, &$form_state) {
  variable_set('fb_post_app_id', $form_state['values']['app_id']);
  variable_set('fb_post_app_secret', $form_state['values']['app_secret']);
  drupal_set_message(t('The Facebook App preferences have been saved'));
}

/********************************* Node Settings ******************************/

/**
 * Additional submit handler for node_type_form.
 */
function fb_post_node_type_form_submit($form, &$form_state) {
  $node_type = $form['#node_type']->type;

  foreach ($form_state['values']['fb_post'] as $var => $value) {
    $var_name = 'fb_post_' . $var;
    $enabled = variable_get($var_name, array());
    if ($value) {
      if (($enabled && !in_array($node_type, $enabled)) || !$enabled) {
        $enabled[] = $node_type;
      }
    }
    else {
      if ($enabled && in_array($node_type, $enabled)) {
        $key = array_keys($enabled, $node_type);
        unset($enabled[$key[0]]);
      }
    }

    variable_set($var_name, $enabled);
  }
}

/********************************* User Settings ******************************/

/**
 * Callback function for 'user/%user/edit/fb-post' path.
 *
 * Displays either link for adding connection to FB account or button for removing it.
 *
 * @param stdClass $account
 *   Drupal user object.
 */
function fb_post_page($account) {
  global $user;
  $output = '';

  if ($user->uid == $account->uid) {
    module_load_include('inc', 'fb_post');

    $fb_uid = fb_post_connected($account);
    if ($fb_uid) {
      $remove_connection_form = drupal_get_form('fb_post_remove_connection_form');
      $user_info = fb_post_load_saved_user_info($account);
      if ($user_info) {
        $vars = array(
          'user_info' => $user_info,
          'remove_connection_button' => drupal_render($remove_connection_form),
        );
        $output .= theme('fb_post_user_info', $vars);
      }
      else {
        $output .= drupal_render($remove_connection_form);
      }
      $page_id_form = drupal_get_form('fb_post_page_id_form', $fb_uid);
      $output .= drupal_render($page_id_form);
    }
    else {
      $app_id = variable_get('fb_post_app_id', '');
      if ($app_id) {
        $output = theme('fb_post_connect', array('connection_path' => fb_post_get_oauth_url($account, $app_id)));
      }
      else {
        $output = theme('fb_post_request_app_settings', array('account' => $account));
      }
    }
  }

  return $output;
}

/**
 * FB account connection removal form.
 */
function fb_post_remove_connection_form($form, &$form_state) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Disconnect'),
  );

  return $form;
}

/**
 * Submit handler for fb_post_remove_connection_form.
 */
function fb_post_remove_connection_form_submit($form, &$form_state) {
  global $user;
  fb_post_remove_connection($user);
}

/**
 * FB Page ID to publishing in Page instead of in User form.
 *
 * @param int $fb_uid
 *   Facebook user's account ID.
 */
function fb_post_page_id_form($form, &$form_state, $fb_uid) {
  module_load_include('inc', 'fb_post');
  $fb_page_id = fb_post_get_fb_page_id($fb_uid);

  $form_state['fb_uid'] = $fb_uid;
  $form['fb_page_id'] = array(
    '#type' => 'textfield',
    '#default_value' => $fb_page_id,
    '#title' => t('Page ID'),
    '#description' => t('Fill this field to share in your Page instead of Profile.'),
    '#size' => 20,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('fb_post_page_id_form_submit'),
  );
  if (!fb_post_user_access(FB_POST_FB_PERM_MANAGE_PAGES)) {
    $form['submit']['#value'] = t('Request additional permissions');
    $form['submit']['#submit'][] = 'fb_post_page_id_form_request_submit';
  }

  return $form;
}

/**
 * Submit handler for fb_post_page_id_form.
 */
function fb_post_page_id_form_submit($form, &$form_state) {
  module_load_include('inc', 'fb_post');
  if (fb_post_save_fb_page_id($form_state['values']['fb_page_id'], $form_state['fb_uid'])) {
    drupal_set_message(t('Facebook preferences have been saved'));
  }
  else {
    drupal_set_message(t("Sorry, your Facebook preferences haven't been saved. Please check that you connected to Facebook."));
  }
}

/**
 * Submit handler for fb_post_page_id_form. Request additional permissions.
 */
function fb_post_page_id_form_request_submit($form, &$form_state) {
  global $user;
  if ($app_id = variable_get('fb_post_app_id', '')) {
    $form_state['redirect'] = fb_post_get_oauth_url($user, $app_id);
  }
}

/**
 * Callback function for 'user/%user/edit/fb-post/redirect-code' path.
 *
 * Takes care of obtaining the user access token and saving it.
 *
 * @param stdClass $account
 *   Drupal user object.
 */
function fb_post_redirect_page($account) {
  $code = (isset($_GET['code'])) ? $_GET['code'] : '';

  if ($code) {
    $app_id = variable_get('fb_post_app_id', '');
    $app_secret = variable_get('fb_post_app_secret', '');
    $app_access_token = variable_get('fb_post_app_access_token', '');

    $redirect_uri = url('user/' . $account->uid . '/edit/fb-post/redirect-code', array('absolute' => TRUE));
    $path = 'https://graph.facebook.com/oauth/access_token?client_id=' . $app_id . '&redirect_uri=' . $redirect_uri
          . '&client_secret=' . $app_secret . '&code=' . $code;
    $token_str = @file_get_contents($path);
    if ($token_str) {
      parse_str($token_str);
      // If $access_token was extracted from the $token_str
      if (!empty($access_token)) {
        module_load_include('inc', 'fb_post');

        $token_data = fb_post_inspect_user_access_token($access_token, $app_access_token);
        if ($token_data) {
          fb_post_save_user_info($account, $access_token, $token_data);
        }
      }
    }
    drupal_goto('user/' . $account->uid . '/edit/fb-post');
  }
  else {
    drupal_set_message(t('Could not obtain access code from Facebook. Please try again.'), 'error');
    return '';
  }
}
