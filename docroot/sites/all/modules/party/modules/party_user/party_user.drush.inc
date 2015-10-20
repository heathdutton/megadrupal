<?php
/**
 * Drush command file for party_user.
 */

/**
 * Implements hook_drush_command().
 */
function party_user_drush_command() {
  $items = array();

  $items['party-login'] = array(
    'callback' => 'drush_party_user_login',
    'description' => 'Display a one time login link for the given party if an attached user is found.',
    'aliases' => array('pli'),
    'required-arguments' => 1,
    'arguments' => array(
      'party' => 'The id of the party we want to generate a link for.',
      'path' => 'Optional path to redirect to after logging in.',
    ),
    'examples' => array(
      'drush party-user-login 2 user' => 'Displays a one-time login link for the user account attached to party 2 and redirects to the path user.',
    ),
  );

  return $items;
}

/**
 * Displays a one time login link for the given party.
 *
 * @param $pid
 *   The party we want to generate a link for.
 * @param $path
 *   Optional path to redirect to after logging in.
 */
function drush_party_user_login($pid, $path = NULL) {
  // Load the party.
  if (!$party = party_load($pid)) {
    drush_set_error('The party could not be loaded.');
    return;
  }

  if (!($user = $party->getDataSetController('user')->getEntity())) {
    drush_set_error('The given party has no associated user.');
    return;
  }

  // Pass onto drush_user_login.
  return drush_user_login($user->uid, $path);
}
