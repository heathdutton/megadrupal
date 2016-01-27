<?php
/**
 * @file
 * Platform sign-in functions
 */

/**
 * Helper function to set up the Drupal account and run some checks for the oauth endpoint
 */
function _janrain_capture_setup_local_account($account, $profile) {
  $ver = variable_get('janrain_capture_ver', JANRAIN_CAPTURE_VERSION_DEFAULT);

  if ($ver == JANRAIN_CAPTURE_VERSION_LEGACY) {
    $janrain_capture_fields   = variable_get('janrain_capture_fields', array());
  } else {
    $janrain_capture_fields   = variable_get('janrain_capture_fields2', array());
  }

  $store_email = !isset($janrain_capture_fields['capture_no_email']) || !$janrain_capture_fields['capture_no_email'];

  if (!isset($account->uid)) {
    // No user was found with our Capture uuid. If we store email addresses in the
    // Drupal database, we can also try matching based on email.
    if ($store_email) {
      // Look for a local user with the same email address.
      $loaded_user = user_load_multiple(array(), array('mail' => $profile['result']['email']));
      $local_user = reset($loaded_user);
    }
    if ($store_email && $local_user) {
      // Are we configured to match users based on email?
      if (isset($janrain_capture_fields['capture_match_email']) && $janrain_capture_fields['capture_match_email']) {
        // Check to see if this user is already mapped to a Capture uuid.
        if (janrain_capture_mapping_exists($local_user->uid)) {
          $mapped_hook = module_invoke_all('janrain_capture_user_already_mapped');
          if (empty($mapped_hook) || !in_array(FALSE, $mapped_hook)) {
            drupal_set_message(t('A user with this email address is already mapped.'), 'error');
          }
        }
        // Bind Capture profile data and uuid to existing user record.
        else {
          user_set_authmaps($local_user, array("authname_janrain_capture" => $profile['result']['uuid']));
          $account = $local_user;
        }
      }
      else {
        $account = FALSE;
        $user_exists_hook = module_invoke_all('janrain_capture_user_exists');
        if (empty($user_exists_hook) || !in_array(FALSE, $user_exists_hook)) {
          drupal_set_message(t('A user with this email address already exists.'), 'error');
        }
      }
    }
    // No local users found. Create a new one.
    else {
      $user_info['pass'] = user_password();
      $account = user_save($account, $user_info);
      $new_user = TRUE;
      if (!$account->uid) {
        $failed_create = module_invoke_all('janrain_capture_failed_create');
        if (empty($failed_create) || !in_array(FALSE, $failed_create)) {
          drupal_set_message(t('Failed to create new user.'), 'error');
        }
      }
      else {
        user_set_authmaps($account, array("authname_janrain_capture" => $profile['result']['uuid']));
      }
    }
  }
  return $account;
}
