<?php
/**
 * @file
 *  Page callbacks for the og_default_members module.
 */


/**
 * Menu callback; Autocomplete callback to retrieve user name suggestions
 *
 * @param string $users_typed
 *
 * @return Array $matches
 */
function og_default_members_autocomplete($users_typed) {
  $matches = array();
  // The user enters a comma-separated list of users.
  $users_typed = drupal_explode_tags($users_typed);
  $users_last = drupal_strtolower(array_pop($users_typed));

  if (!empty($users_last)) {
    $result = db_select('users')
      ->fields('users', array('name'))
      ->condition('name', db_like($users_last) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();

    $prefix = count($users_typed) ? drupal_implode_tags($users_typed) . ', ' : '';
    foreach ($result as $user) {
      $matches[$prefix . $user->name] = check_plain($user->name);
    }
  }

  return drupal_json_output($matches);
}
