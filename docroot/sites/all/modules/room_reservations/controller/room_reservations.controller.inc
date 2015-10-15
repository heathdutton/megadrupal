<?php
/**
 * @file
 * Miscellaneous controller functionality.
 */

/**
 * Display a list of the user's current reservations.
 *
 * @global object $user
 *   Drupal user object.
 * @global type $base_url
 *   The base URL of the Drupal installation.
 *
 * @return string
 *   A list of the user's reservations in HTML format.
 */
function room_reservations_res_list() {
  global $user;
  global $base_url;
  $user_reservations = NULL;
  $count = 0;
  if ($user->uid) {
    // User is logged in.
    $user_reservations = _room_reservations_user_reservations();
    $count = count($user_reservations);
  }
  $output .= theme('room_reservations_list', array('user' => $user, 'base_url' => $base_url, 'user_reservations' => $user_reservations, 'count' => $count));
  return $output;
}

/**
 * Display a page showing the reservation system policies.
 *
 * @return string
 *   The policies for the reservation system as entered by the administrator
 *   using the Display Text configuration page.
 */
function room_reservations_policies() {
  $output = check_markup(_room_reservations_get_variable('policies'));
  return $output;
}

