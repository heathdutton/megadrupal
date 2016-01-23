<?php

/**
 * @file
 * Hooks provided by the Flocknote module.
 *
 * This file contains no working PHP code; it exists to provide additional
 * documentation as well as to document hooks in the standard Drupal manner.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter user information before submitting to Flocknote as a new Member.
 *
 * This hook is called after a new user account is created, and grabs the user's
 * email address and the Flocknote List ID to which the user should be
 * subscribed, and then uses the data to add the member to Flocknote.
 *
 * You may wish to use this hook to add in a first_name and last_name before
 * passing along the information to Flocknote.
 *
 * This is called from flocknote_subscribe_user_to_list().
 *
 * @param $member (array)
 *   - email: The user's email address.
 *   - list_id: The Flocknote list_id to which the user will be subscribed.
 */
function hook_flocknote_member_alter(&$member) {
  $member['first_name'] = 'John';
  $member['last_name'] = 'Doe';
}

/**
 * @} End of "addtogroup hooks".
 */
