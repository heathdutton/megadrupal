<?php

/**
 * @file
 * Usergroups API integration for user.module.
 */

/**
 * Implementation of hook_usergroups_api_options() for 'user'.
 * Ideally this function will never be called as it's going to load
 * every user in the database, but we must implement this function.
 */
function user_usergroups_api_user_options() {
  static $options;
  if (!is_array($options)) {
    $options = array();
    $results = db_query('SELECT u.* FROM {users} u WHERE u.status=1');
    foreach ($results as $r) {
      $options[$r->uid] = $r->name;
    }
  }
  return $options;
}

/**
 * Implementation of hook_usergroups_api_options_title() for 'user'.
 * Ideally this function will always be called.
 */
function user_usergroups_api_user_options_title($uid) {
  if ($r = db_query('SELECT u.* FROM {users} u WHERE u.uid=:uid', array(':uid' => $uid))->fetchObject()) return $r->name;
  return '';
}

/**
 * Implementation of hook_usergroups_api_specific_groups() for 'user'.
 */
function user_usergroups_api_user_specific_groups($account) {
  return array($account->uid => $account->name);
}

/**
 * Implementation of hook_usergroups_api_options_form() for 'user'.
 */
function user_usergroups_api_user_options_form(&$form, &$form_state, $options) {
  $form_item = array(
    '#type' => 'textfield',
    '#description' => t('Begin entering the user\'s username that you would like to select.'),
    '#autocomplete_path' => 'usergroups_api/user/autocomplete',
  );
  if ($options['multiples_per_group']) {
    $form_item['#description'] .= ' ' . t('Separate multiple usernames with a comma.');
  }

  // Convert the uids to usernames.
  $names = array();
  if (isset($options['defaults_per_type']['original']['user'])) {
    foreach ($options['defaults_per_type']['original']['user'] as $uid) {
      $names[] = _user_usergroups_api_user_get_username($uid);
    }
  }
  $form_item['#default_value'] = implode(',', $names);
  return $form_item;
}

/**
 * Implementation of hook_usergroups_api_options_form_validate() for 'user'.
 */
function user_usergroups_api_user_options_form_validate(&$form, &$form_state) {
  $users = $groups = array();
  $names_entered = explode(',', $form_state['values']['usergroups_api_group_user']);
  foreach ($names_entered as $name) {
    $name = trim($name);
    // Only allow a username once and do not allow blank usernames.
    if (!$name || isset($users[$name])) {
      continue;
    }
    $users[$name] = _user_usergroups_api_user_get_uid($name);
    // As long as the user was found, add them to the list of groups.
    if ($users[$name]) {
      $groups[$users[$name]] = $users[$name];
    }
  }
  return $groups;
}

/**
 * Autcomplete callback function to handle mutliple users.
 */
function user_usergroups_api_user_options_form_autocomplete_callback($string) {
  $all_vals = explode(',', $string);
  $last_string = trim(array_pop($all_vals));
  $string = implode(',', $all_vals);
  if ($string) {
    $string .= ',';
  }
  $matches = array();
  $result = db_query_range("SELECT LOWER(u.name) as name FROM {users} u WHERE LOWER(u.name) LIKE LOWER(:string) AND u.status=1", 0, 10, array(
    ':string' => $last_string . '%',
  ));
  foreach ($result as $user) {
    $matches[$string . $user->name . ','] = check_plain($user->name);
  }
  echo drupal_json_encode($matches);
  exit();
}

/**
 * Callback function to get the user's username.
 */
function _user_usergroups_api_user_get_username($uid) {
  return db_query("SELECT LOWER(u.name) FROM {users} u WHERE u.uid=:uid", array(':uid' => $uid))->fetchField();
}

/**
 * Callback function to get the user's username.
 */
function _user_usergroups_api_user_get_uid($name) {
  return db_query("SELECT u.uid FROM {users} u WHERE LOWER(u.name)=LOWER(:string)", array(':string' => $name))->fetchField();
}



/**
 * Implementation of hook_usergroups_api_options() for 'role'.
 */
function user_usergroups_api_role_options() {
  static $roles;
  if (!is_array($roles)) {
    $roles = user_roles();
  }
  return $roles;
}

/**
 * Implementation of hook_usergroups_api_specific_groups() for 'role'.
 */
function user_usergroups_api_role_specific_groups($account) {
  return $account->roles;
  $roles = user_usergroups_api_role_options();
  $groups = array();
}
