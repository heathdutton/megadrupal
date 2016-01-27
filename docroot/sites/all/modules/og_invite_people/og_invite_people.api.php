<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 * 
 */

/**
 * Allows you to take action when a new user is invited to OG group
 * through OG Invite People.
 * 
 * @param $account
 *   The user object for the account associated with the group.
 * @param $og_membership
 *   The OG membership object.
 */
function hook_og_invite_people_invited($account, $og_membership) {
  // Example.
  // drupal_set_message('We have a new member.');
}
