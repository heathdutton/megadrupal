<?php
/**
 * @file
 * API calls for use across all networks.
 */

/**
 * Hook to provide UID comparison list from a network.
 *
 * @param int $uid
 *   A Drupal user UID.
 *
 * @return array
 *   An associative array of possible user id matches and score to act upon.
 *
 * @see user_relationship_locator_matches().
 */
function hook_urlocator_user_query($uid) {
  $score = 99;
  $users = array(
    1 => $score, // User id = 1
    2 => $score, // User id = 2
    3 => $score, // User id = 3
  );

  return $users;
}

/**
 * Hook to explain additional buttons.
 *
 * @return array
 *   An associative array of buttons.
 *
 * @see user_relationship_locator_preprocess_user_relationship_locator_load_buttons().
 */
function hook_urlocator_load_buttons() {
  $buttons = array();

  $buttons['facebook'] = l('Facebook', 'http://facebook.com');

  return $buttons;
}

/**
 * Alter the list of buttons provided through hook_urlocator_load_buttons().
 *
 * @param array $buttons
 *   An associative array of buttons.
 *
 * @see user_relationship_locator_preprocess_user_relationship_locator_load_buttons().
 */
function hook_urlocator_load_buttons_alter(&$buttons) {
  unset($buttons['facebook']);
}

/**
 * Hook runs after data has been imported to DB.
 *
 * @param object $account
 *   User object.
 * @param string $module
 *   Module name who invoked hook.
 *
 * @see user_relationship_locator_facebook_fboauth_users().
 */
function hook_urlocator_data_imported($account, $module = '') {
  // Get suggestions.
  $matches = module_invoke($module, 'urlocator_user_query', $account->uid);

  if (!empty($matches)) {
    $matches = array_keys($matches);
    $users = user_load_multiple($matches);
    // Do something with users.
  }
}
