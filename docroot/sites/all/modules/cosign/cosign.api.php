<?php

/**
 * @file
 * Hooks provided by Cosign module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Add behavior when Cosign remote user matches local Drupal user.
 *
 * When the user who is logged into Cosign remotely matches the user also logged
 * into Drupal locally, this hook is invoked. When this happens, Cosign module
 * exits out of its authentication checks and allows Drupal to proceed.
 *
 * @param $local_user
 *   Drupal user object for local Drupal user who is presently logged in.
 */
function hook_cosign_remote_local_match($local_user) {

  drupal_set_message(t('You\'re logged in as %local.', array('%local' => $local_user->name)));

}

/**
 * Control behavior when Cosign remote user does not match local Drupal user.
 *
 * When a user is logged into Cosign remotely and also logged into Drupal
 * locally, it is possible for the remote user and the local user to mis-match.
 * When this happens, by default the local Drupal user is logged out before
 * proceeding to log in the corresponding local Drupal user to the remote Cosign
 * user. The default behavior can be modified to allow the local user to remain
 * a mis-match of the remote user by utilizing this hook.
 *
 * Take note that the last module to invoke this hook according to its weight is
 * the one whose return value will dictate whether or not the user is logged out
 * on mis-match.
 *
 * @param $local_user
 *   Drupal user object for local Drupal user who is presently logged in.
 * @param $remote_user
 *   Drupal user object for remote Cosign user who is presently logged in to
 *   Cosign.
 *
 * @return
 *   Boolean TRUE to proceed with default behavior to log user out, FALSE to
 *   allow local user to remain logged in despite remote user mismatch.
 */
function hook_cosign_remote_local_mismatch($local_user, $remote_user) {

  drupal_set_message(t('Warning: You are impersonating another user. You are %remote, you are impersonating %local.', array('%remote' => $remote_user->name, '%local' => $local_user->name)));

  return FALSE;

}

/**
 * Add behavior when new local Drupal user is created by Cosign module.
 *
 * When a user is logged into Cosign remotely but no corresponding local Drupal
 * user exists and Cosign module creates a new local Drupal user automatically,
 * this hook is invoked to allow the opportunity to add additional
 * functionality. Note: the cosign_autocreate variable option must be checked
 * in the Cosign module configuration for this hook to fire. If not, a new user
 * is not created.
 *
 * @param $new_user
 *   Drupal user object for new Drupal user who was just created.
 */
function hook_cosign_user_created($new_user) {

  drupal_set_message(t('A new user was just created for you.'));

}

/**
 * Add behavior when new local Drupal user is not created by Cosign module.
 *
 * When a user is logged into Cosign remotely but no corresponding local Drupal
 * user exists and Cosign module is not configured to create a new local Drupal
 * user, this hook is invoked to allow the opportunity to add additional
 * functionality. Note: the cosign_autocreate variable option must not be
 * checked in the Cosign module configuration for this hook to fire. If not, a
 * new user will be created.
 *
 * @param $remote_username
 *   Username of the logged in remote Cosign user.
 */
function hook_cosign_user_not_created($remote_username) {

  drupal_set_message(t('A new user was not able to be created for you.'));

}

/**
 * Add behavior when no remote user is logged in via Cosign module.
 *
 * When no remote user is detected, this hook is invoked.
 */
function hook_cosign_no_remote_user() {

  drupal_set_message(t('No remote Cosign user is logged in.'));

}

/**
 * @} End of "addtogroup hooks".
 */
