<?php

/**
 * Preprocesses variables for theme_username().
 *
 * Modules that make any changes to variables like 'name' or 'extra' must insure
 * that the final string is safe to include directly in the output by using
 * check_plain() or filter_xss().
 *
 * @see twbs_process_username()
 */
function twbs_preprocess_username(&$variables) {
  $account = $variables['account'];

  $variables['extra'] = '';
  if (empty($account->uid)) {
   $variables['uid'] = 0;
   if (theme_get_setting('toggle_comment_user_verification')) {
     $variables['extra'] = ' (' . t('not verified') . ')';
   }
  }
  else {
    $variables['uid'] = (int) $account->uid;
  }

  // Set the name to a formatted name that is safe for printing and
  // that won't break tables by being too long. Keep an unshortened,
  // unsanitized version, in case other preprocess functions want to implement
  // their own shortening logic or add markup. If they do so, they must ensure
  // that $variables['name'] is safe for printing.
  $name = $variables['name_raw'] = format_username($account);
  if (drupal_strlen($name) > 20) {
    $name = drupal_substr($name, 0, 15) . '...';
  }
  $variables['name'] = check_plain($name);

  $variables['profile_access'] = user_access('access user profiles');
  $variables['link_attributes'] = array();
  // Populate link path and attributes if appropriate.
  if ($variables['uid'] && $variables['profile_access']) {
    // We are linking to a local user.
    $variables['link_attributes'] = array('title' => t('View user profile.'));
    $variables['link_path'] = 'user/' . $variables['uid'];
  }
  elseif (!empty($account->homepage)) {
    // Like the 'class' attribute, the 'rel' attribute can hold a
    // space-separated set of values, so initialize it as an array to make it
    // easier for other preprocess functions to append to it.
    $variables['link_attributes'] = array('rel' => array('nofollow'));
    $variables['link_path'] = $account->homepage;
    $variables['homepage'] = $account->homepage;
  }
  // We do not want the l() function to check_plain() a second time.
  $variables['link_options']['html'] = TRUE;
  // Set a default class.
  $variables['attributes_array'] = array('class' => array('username'));
}
