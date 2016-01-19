<?php
/**
 * @file
 * Defines avatar data functionality.
 */

/**
 * Page callback to get avatar data.
 */
function drpg_avatar_data() {
  global $user;

  $active_avatar = drpg_avatar_get_user_active_avatar($user->uid);
  if ($active_avatar === FALSE) {
    // Game clients expect an object, don't send boolean value.
    $active_avatar = NULL;
  }

  $response = new DrpgAvatarResponse();
  $response->success = ($active_avatar != NULL);
  $response->avatar = $active_avatar;

  drpg_json_output($response);
}
