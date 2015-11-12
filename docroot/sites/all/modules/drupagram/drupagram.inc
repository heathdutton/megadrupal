<?php

/**
 * @file
 * Instagram API functions
 */

/**
 * Connect to the API using the 'proper' version (Oauth vs. standard)
 */
function drupagram_connect($account) {
  $auth = $account->get_auth();
  if (_drupagram_use_oauth() && $auth['oauth_token'] && $auth['oauth_token_secret']) {
    module_load_include('lib.php', 'oauth_common');
    return new InstagramOAuth(variable_get('drupagram_consumer_key', ''), variable_get('drupagram_consumer_secret', ''),
        $auth['oauth_token'], $auth['oauth_token_secret']);
  }
  elseif (_drupagram_use_oauth() && $auth['oauth_token']) {
    return new Instagram($account->username, $auth['oauth_token']);
  }
  else {
    return new Instagram;
  }
}

/**
 * Saves a InstagramUser object to {drupagram_account}
 */
function drupagram_account_save($drupagram_user, $save_auth = FALSE, $account = NULL) {
  $values = (array) $drupagram_user;

  if (empty($drupagram_user)) {
    return FALSE;
  }

  $values['drupagram_id'] = $values['id'];

  if ($save_auth) {
    $values += $drupagram_user->get_auth();
    if (empty($account)) {
      global $user;
      $account = $user;
    }
    $values['uid'] = $account->uid;
  }

  $schema = drupal_get_schema('drupagram_account');

  foreach ($values as $k => $v) {
    // Remove values that we don't have defined in our schema
    if (!isset($schema['fields'][$k])) {
      unset($values[$k]);
    }
    // Convert nulls to empty strings
    elseif (isset($schema['fields'][$k]['not null']) && $schema['fields'][$k]['not null'] && empty($values[$k])) {
      $values[$k] = isset($schema['fields'][$k]['default']) ? drupagram_emoji($schema['fields'][$k]['default']) : '';
    }
  }

  db_merge('drupagram_account')
    ->key(array('drupagram_id' => $values['drupagram_id']))
    ->fields($values)
    ->execute();
}

/**
 * Load a Instagram account from {drupagram_account}.
 *
 * @param $id
 *   Instagram UID
 *
 * @return
 *   InstagramUser object
 *
 */
function drupagram_account_load($id) {
  if ($values = db_query("SELECT * FROM {drupagram_account} WHERE drupagram_id = :drupagram_id", array(':drupagram_id' => $id))->fetchAssoc()) {
    $values['id'] = $values['drupagram_id'];
    $account = new InstagramUser($values);
    $account->set_auth($values);
    $account->uid = $values['uid'];
    $account->import = $values['import'];
    $account->is_global = $values['is_global'];
    return $account;
  }
}

/**
 * Saves a InstagramMedia object to {drupagram}
 */
function drupagram_media_save($media) {
  $drupagram = array(
    'drupagram_id' => check_plain($media->id),
    'username' => drupagram_emoji($media->user->username),
    'type' => filter_xss($media->type),
    // 'tags' => serialize($media->tags),
    'images' => serialize($media->images),
    'location' => serialize($media->location),
    'comments' => serialize(drupagram_emoji($media->comments)),
    'caption' => serialize(drupagram_emoji($media->caption)),
    'link' => drupagram_emoji($media->link),
    'likes' => serialize(drupagram_emoji($media->likes)),
    'filter' => filter_xss($media->filter),
    'created_at' => format_date($media->created_time, 'long'),
    'created_time' => $media->created_time,
  );

  // Filter out any empty keys in our array.
  $drupagram = array_filter($drupagram);

  db_merge('drupagram')
    ->key(array('drupagram_id' => $drupagram['drupagram_id']))
    ->fields($drupagram)
    ->execute();
}

/**
 * Post a message to drupagram
 */
function drupagram_set_status($drupagram_account, $status) {

}

/**
 * Fetches a user's feed
 */
function drupagram_fetch_user_feed($id) {
  $params = array();
  $account = drupagram_account_load($id);

  $min_id = db_query("SELECT MAX(drupagram_id) FROM {drupagram} WHERE username = :username", array(':username' => $account->username))->fetchField();

  $params['!min_id'] = (int) $min_id;
  $params['count'] = 6;

  $drupagram = drupagram_connect($account);

  $items = $drupagram->user_recent($id);

  if (empty($items)) {
    return;
  }

  foreach ($items as $media) {
    drupagram_media_save($media);
  }

  if (count($items) > 0) {
    // drupagram_account_save($items[0]->user);
  }

  db_update('drupagram_account')
    ->fields(array(
      'last_refresh' => REQUEST_TIME,
    ))
    ->condition('drupagram_id', $account->id)
    ->execute();
}

/**
 * Fetches a user's recent items.
 */
function drupagram_fetch_recent_items($id) {
  $params = array();
  $account = drupagram_account_load($id);

  $min_id = db_query("SELECT MAX(drupagram_id) FROM {drupagram} WHERE username = :username", array(':username' => $account->username))->fetchField();

  $params['!min_id'] = (int) $min_id;
  $params['count'] = 6;

  $drupagram = drupagram_connect($account);

  $items = $drupagram->user_recent($id);

  if (empty($items)) {
    return;
  }

  foreach ($items as $media) {
    drupagram_media_save($media);
  }

  if (count($items) > 0) {
    // drupagram_account_save($items[0]->user);
  }

  db_update('drupagram_account')
    ->fields(array(
      'last_refresh' => REQUEST_TIME,
    ))
    ->condition('drupagram_id', $account->id)
    ->execute();
}

/**
 * Delete a drupagram account and its statuses.
 *
 * @param $drupagram_id
 *   An integer with the Instagram UID.
 *
 * @param $username
 *   Optional string with the user name.
 */
function drupagram_account_delete($drupagram_id) {
  $account = drupagram_account_load($drupagram_id);

  // Delete from {drupagram_account}.
  $query = db_delete('drupagram_account');
  $query->conditions('drupagram_id', $drupagram_id);
  $query->execute();

  // Delete from {drupagram}.
  $query = db_delete('drupagram');
  $query->conditions('username', $account->username);
  $query->execute();

  // Delete from {drupagram_account}.
  $query = db_delete('authmap');
  $query->conditions('authname', $drupagram_id);
  $query->conditions('module', 'drupagram');
  $query->execute();
}

/**
 * Helper function to handle emoji special characters.
 *
 * In the future this will be the single point where emoji unicode characters
 * will be replaced with the actual image for it.
 *
 * This issue is only pertinent to users adding emoji characters to their posts
 * or usernames.
 */
function drupagram_emoji(&$variable) {
  if (is_array($variable)) {
    foreach ($variable as $key => $value) {
      if (empty($value)) {
        continue;
      }
      $variable[$key] = drupagram_emoji($value);
    }
  }
  elseif (is_object($variable)) {
    $variable = (array) $variable;
    foreach ($variable as $key => $value) {
      if (empty($value)) {
        continue;
      }
      $variable[$key] = drupagram_emoji($value);
    }
    $variable = (object) $variable;
  }
  elseif (is_string($variable)) {
    $variable = preg_replace("/[\xC0-\xF7]{1,1}[\x80-\xBF]+/", "", htmlspecialchars($variable, ENT_NOQUOTES, 'UTF-8', FALSE));
  }
  return $variable;
}
