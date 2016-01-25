<?php

/**
 * @file
 * Contains Account Sentinel's templates and template preprocessors.
 */

/**
 * Formats a username by its UID.
 *
 * Almost the same as the "username" theme, but if the UID is not 0 and
 * user_load returns FALSE, it will return the UID of the (probably) deleted
 * user.
 *
 * @param array $variables
 *   An associative array containing:
 *   - uid: The UID of the user.
 *
 * @see theme_username()
 *
 * @return string
 *   The HTML for a username.
 */
function theme_account_sentinel_username(array $variables) {
  $uid = $variables['uid'];
  $user = user_load($uid);

  if ($uid != 0 && $user === FALSE) {
    return t('#@uid (deleted)', array('@uid' => $uid));
  }
  else {
    return theme('username', array('account' => $user));
  }
}
