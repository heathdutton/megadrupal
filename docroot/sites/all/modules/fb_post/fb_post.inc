<?php

/**
 * @file
 * Helper functions.
 */

/**
 * Load a FB Page ID.
 *
 * @param int $fb_uid
 *   Facebook user's account ID.
 *
 * @return int
 *   Facebook Page ID.
 */
function fb_post_get_fb_page_id($fb_uid) {
  $result = db_select('fb_post', 'fbp')
    ->fields('fbp', array('fb_page_id'))
    ->condition('fb_uid', $fb_uid, '=')
    ->execute();

  return $result->fetchField();
}

/**
 * Save a FB Page ID.
 *
 * @param int $page_id
 *   Facebook Page ID.
 * @param int $fb_uid
 *   Facebook user's account ID.
 *
 * @return int
 *   Number of updated rows.
 */
function fb_post_save_fb_page_id($page_id, $fb_uid) {
  return db_update('fb_post')
    ->fields(array(
      'fb_page_id' => $page_id,
    ))
    ->condition('fb_uid', $fb_uid, '=')
    ->execute();
}

/**
 * Gets the application access token which will be used for posting messages on the users behalf.
 *
 * @param string $app_id
 *   Facebook App ID.
 * @param string $app_secret
 *   Facebook App secret.
 */
function fb_post_get_app_access_token($app_id, $app_secret) {
  $path = 'https://graph.facebook.com/oauth/access_token?client_id=' . $app_id
    . '&client_secret=' . $app_secret . '&grant_type=client_credentials';
  $token_str = @file_get_contents($path);
  if ($token_str) {
    parse_str($token_str);
    if (!empty($access_token)) {
      return $access_token;
    }
  }
}

/**
 * Checks the received user access token to confirm that the token belongs to
 * the person that our app is expecting it to belong to, and that it was our
 * app that generated the token.
 *
 * @param string $user_access_token
 *   User access token.
 * @param string $app_access_token
 *   Application access token.
 */
function fb_post_inspect_user_access_token($user_access_token, $app_access_token) {
  $path = 'https://graph.facebook.com/debug_token?input_token=' . $user_access_token
    . '&access_token=' . $app_access_token;
  $token_str = @file_get_contents($path);
  if ($token_str) {
    $output = preg_replace('/([^\\\])":([0-9]{10,})(,|})/', '$1":"$2"$3', $token_str);
    $data = json_decode($output);
    if (!empty($data->data)) {
      return $data->data;
    }
  }

  return FALSE;
}

/**
 * Gets info about the user from his/her Facebook account.
 *
 * @param string $fb_uid
 *   Facebook user ID.
 * @param string $type
 *   Type of info to fetch. Can be either 'profile' or 'picture'.
 */
function fb_post_get_user_info($fb_uid, $type = 'profile') {
  if ($type == 'profile') {
    $path = 'https://graph.facebook.com/' . $fb_uid;
  }
  elseif ($type == 'picture') {
    $path = 'http://graph.facebook.com/' . $fb_uid . '/picture?type=normal&redirect=false';
  }
  else {
    return FALSE;
  }

  $user_info = @file_get_contents($path);
  if ($user_info) {
    return json_decode($user_info);
  }

  return FALSE;
}

/**
 * Saves user access token data into the DB for further using.
 *
 * @param stdClass $account
 *   Drupal user.
 * @param string $user_access_token
 *   User access token.
 * @param string $token_data
 *   Data received during user access token check.
 */
function fb_post_save_user_info($account, $user_access_token, $token_data) {
  $record = new stdClass();
  $record->uid = $account->uid;
  $record->fb_uid = $token_data->user_id;
  $record->access_token = $user_access_token;
  $record->access_token_expires = $token_data->expires_at;

  if ($user_info = fb_post_get_user_info($token_data->user_id)) {
    $record->name = $user_info->name;
    if (!empty($user_info->link)) {
      $record->link = $user_info->link;
    }
  }

  if ($picture = fb_post_get_user_info($token_data->user_id, 'picture')) {
    $record->picture = $picture->data->url;
  }

  $record->fb_perm = FB_POST_FB_PERM_PUBLISH_ACTIONS;
  if ($fb_uid = fb_post_connected($account)) {
    if (fb_post_get_fb_page_id($fb_uid)) {
      $record->fb_perm .= ', ' . FB_POST_FB_PERM_MANAGE_PAGES;
    }
  }

  db_merge('fb_post')
    ->key(array('uid' => $record->uid, 'fb_uid' => $record->fb_uid))
    ->fields((array) $record)
    ->execute();
}

/**
 * Fetches saved Facebook account info of given user.
 *
 * @param stdClass $user
 *   Drupal user object.
 */
function fb_post_load_saved_user_info($user) {
  $query = db_select('fb_post')
    ->fields('fb_post', array('fb_uid', 'name', 'link', 'picture', 'access_token_expires'))
    ->condition('uid', $user->uid)
    ->execute();

  return $query->fetchObject();
}

/**
 * Deletes the token from the DB.
 *
 * @param stdClass $user
 *   Drupal user object.
 */
function fb_post_remove_connection($user) {
  $query = db_delete('fb_post')
    ->condition('uid', $user->uid)
    ->execute();
}

/**
 * Checks if given user has FB account connected and if his token is valid if so.
 *
 * @param stdClass $user
 *   Drupal user object.
 */
function fb_post_connected($user) {
  $query = db_select('fb_post')
    ->fields('fb_post', array('fb_uid'))
    ->condition('uid', $user->uid)
    ->execute();
  $fb_uid = $query->fetchField();

  if ($fb_uid) {
    $token_valid = fb_post_user_access_token_valid($user);
    if ($token_valid) {
      return $fb_uid;
    }
    else {
      fb_post_remove_connection($user);
      return FALSE;
    }
  }

  return FALSE;
}

/**
 * Checks if user access token is valid.
 *
 * @param stdClass $user
 *   Drupal user object.
 */
function fb_post_user_access_token_valid($user) {
  $query = db_select('fb_post')
    ->fields('fb_post', array('access_token_expires'))
    ->condition('uid', $user->uid)
    ->execute();
  $token_expires = $query->fetchField();
  if ($token_expires && $token_expires > REQUEST_TIME) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Creates a post in user's news feed on behalf his/her name.
 *
 * @param string $fb_uid
 *   Facebook user's account ID.
 * @param array $post
 *   Array of message data to send to FB.
 *   Something like
 *   array(
 *     'message' => 'text of the message',
 *     'description' => 'description of the message',
 *     'link' => 'http://example.com',
 *   );
 *   @see https://developers.facebook.com/docs/reference/api/post/ for more details.
 * @param stdClass $entity
 *  Enity object (node, comment).
 */
function fb_post_make_post($fb_uid, $post, $entity) {
  $user_access_token = fb_post_get_user_token_by_fbuid($fb_uid);
  $fb_page_id = fb_post_get_fb_page_id($fb_uid);
  if ($fb_page_id) {
    $url = 'https://graph.facebook.com/' . $fb_page_id . '/feed';
  }
  else {
    $url = 'https://graph.facebook.com/' . $fb_uid . '/feed';
  }
  $access_token = array(
    'access_token' => $user_access_token,
  );
  $post_fields = array_merge($access_token, $post);

  drupal_alter('fb_post', $post_fields, $url, $entity);

  $ch = curl_init($url);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post_fields);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  $result = json_decode(curl_exec($ch));
  curl_close($ch);

  if (isset($result->error)) {
    watchdog('Facebook Post', 'Type: "%type"; code: "%code"; message: "%message"', array(
      '%type' => $result->error->type, '%code' => $result->error->code, '%message' => $result->error->message)
      , WATCHDOG_ERROR);
    drupal_set_message(t('An error occurred while sending the message to Facebook. Please contact administrator.'));
  }

  return $result;
}

/**
 * Helper function. Gets user's token.
 *
 * @param int $uid
 *   User ID.
 */
function fb_post_get_user_token($uid) {
  $query = db_select('fb_post')
    ->fields('fb_post', array('access_token'))
    ->condition('uid', $uid)
    ->execute();

  return $query->fetchField();
}

/**
 * Helper function. Gets user's token by his Facebook account ID.
 *
 * @param string $fb_uid
 *   Facebook account ID.
 */
function fb_post_get_user_token_by_fbuid($fb_uid) {
  $query = db_select('fb_post')
    ->fields('fb_post', array('access_token'))
    ->condition('fb_uid', $fb_uid)
    ->execute();

  return $query->fetchField();
}

/**
 * Determine whether the user has a given privilege. All FB permission checks in
 * Drupal should go through this function.
 *
 * @param string $fb_perm_name
 *   The FB permission, such as "publish_actions", being checked for.
 * @param stdClass $account (optional)
 *   The account to check, if not given use currently logged in user.
 *
 * @return boolean
 *   TRUE if the current user has the requested permission.
 */
function fb_post_user_access($fb_perm_name, $account = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }

  // To reduce the number of SQL queries, we cache the user's permissions
  // in a static variable.
  static $drupal_static_fb_perm;
  if (!isset($drupal_static_fb_perm)) {
    $drupal_static_fb_perm['perm'] = &drupal_static(__FUNCTION__);
  }
  $perm = &$drupal_static_fb_perm['perm'];
  if (!isset($perm[$account->uid])) {
    $result = db_select('fb_post', 'fbp')
      ->fields('fbp', array('fb_perm'))
      ->condition('uid', $account->uid, '=')
      ->execute()
      ->fetchObject();
    if (!empty($result->fb_perm)) {
      $perm[$account->uid] = array_flip(explode(', ', $result->fb_perm));
    }
  }

  return isset($perm[$account->uid][$fb_perm_name]);
}

/**
 * Get a redirect url.
 *
 * @param stdClass $account
 *   Drupal user.
 *
 * @return string
 *   Absolute path.
 */
function fb_post_get_redirect_url($account) {
  return url('user/' . $account->uid . '/edit/fb-post/redirect-code', array('absolute' => TRUE));
}

/**
 * Get an oauth url.
 *
 * @param stdClass $account
 *   Drupal user.
 * @param string $app_id
 *   Facebook application ID.
 *
 * @return string
 *   Absolute path.
 */
function fb_post_get_oauth_url($account, $app_id) {
  $url = 'https://www.facebook.com/dialog/oauth?client_id=' . $app_id . '&redirect_uri='
    . fb_post_get_redirect_url($account) . '&response_type=code&scope='
    . FB_POST_FB_PERM_PUBLISH_ACTIONS;
  if ($fb_uid = fb_post_connected($account)) {
    if (fb_post_get_fb_page_id($fb_uid)) {
      $url .= ', ' . FB_POST_FB_PERM_MANAGE_PAGES;
    }
  }

  return $url;
}
