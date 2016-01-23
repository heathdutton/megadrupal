<?php

/**
 * @file
 * Hooks provided by the Xing Connect module.
 */
/**
 * @addtogroup hooks
 * @{
 */

/**
 * This hook allows other modules to change $fields array before new user is created.
 *
 * @param $fields
 *   The fields array to be stored with user profile in user_save.
 *   Modify these values with values from $xing_user_profile to populate
 *   User Profile with values from Xing while creating new user.
 *
 * @param $xing_candidate_info
 *   Xing GraphObject representing the user (response to "/v1/users" API request)
 *   See: https://dev.xing.com/docs/resources
 *
 */
function hook_xing_connect_register_alter(&$fields, $xing_candidate_info) {
  // Implement this hook in your own module to modify $fields array
}
