<?php

/**
 * Implements hook_email_required_access_alter().
 * 
 * Alter which users are considered to have verified email addresses
 * 
 * @see
 *   email_required_user_is_validated()
 *
 * @param $account
 *   The user account, or uid, we're checking
 * @param $admin
 *   TRUE if users with adequate permissions should be allowed to bypass
 * @param &$access
 *   The current access status for this user. Modify this.
 */
function hook_email_required_access_alter($account, $admin, &$access) {
  if ($account->uid == 5) {
    $access = FALSE;
  }
}
