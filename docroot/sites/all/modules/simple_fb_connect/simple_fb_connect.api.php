<?php

/**
 * @file
 * Hooks provided by the Simple FB Connect module.
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
 *   Modify these values with values from $fb_user_profile to populate
 *   User Profile with values from FB while creating new user.
 *
 * @param $fb_user_profile
 *   Facebook GraphObject representing the user (response to "/me" API request)
 *   See: https://developers.facebook.com/docs/php/GraphObject/4.0.0
 *
 */
function hook_simple_fb_connect_register_alter(&$fields, $fb_user_profile) {
  // Implement this hook in your own module to modify $fields array
}

/**
 * This hook allows other modules to add permissions to $scope array
 *
 * $scope[] = 'email' is added automatically by simple_fb_connect
 * Please note that if your app requires some additional permissions, you may
 * have to submit your Facebook App to Facebook Review process
 *
 * Read more about FB review process:
 * https://developers.facebook.com/docs/apps/review/login
 *
 * @param $scope
 *   The scope array listing the permissions requested by the app
 *
 * @return
 *   The updated scope array
 */
function hook_simple_fb_connect_scope_info($scope) {
  // Implement this hook in your own module to add items to $scope array
  return $scope;
}
