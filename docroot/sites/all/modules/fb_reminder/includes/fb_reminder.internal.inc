<?php

/**
 * @file
 *  Internal functions.
 */

/**
 * Helper function. Get a remind message.
 */
function _fb_reminder_get_message() {
  return variable_get('fb_reminder_template_message');
}

/**
 * Helper function. Get a remind roles.
 */
function _fb_reminder_get_roles() {
  return variable_get('fb_reminder_template_roles', array());
}

/**
 * Helper function. Get a remind roles.
 */
function _fb_reminder_get_pages() {
  return variable_get('fb_reminder_template_access_pages');
}

/**
 * Helper function. Get a remind roles.
 */
function _fb_reminder_get_visibility() {
  return variable_get('fb_reminder_template_visibility', 0);
}
/**
 * Helper function. Check can we display the reminder form
 * on the current page or not.
 *
 * @param stdClass $account
 *  An user object.
 * @return boolean
 */
function _fb_reminder_check_conditionals($account, $url) {
  module_load_include('inc', 'fb_post');
  if (!_fbreminder_is_connected($account)) {
    if (_fb_reminder_check_roles($account)) {
      if (_fb_reminder_check_pages($url)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 * Check an user is connected.
 *
 * @param stdClass $account
 *  An user object.
 * @return boolean
 */
function _fbreminder_is_connected($account) {
  if (module_exists('fb_post')) {
    return fb_post_connected($account);
  }

  return FALSE;
}

/**
 * Check has an user required roles.
 *
 * @param stdClass $account
 *  An user object.
 * @return boolean
 */
function _fb_reminder_check_roles($account) {
  $roles = _fb_reminder_get_roles();
  if ($roles) {
    return array_intersect_key($account->roles, $roles);
  }
}

/**
 * Check has an user required roles.
 *
 * @param string $url
 * @return boolean
 */
function _fb_reminder_check_pages($url) {
  $page_match = FALSE;
  $visibility = _fb_reminder_get_visibility();
  $pages = drupal_strtolower(_fb_reminder_get_pages());

  if ($visibility != FB_REMINDER_FORM_PHP_FILTER) {
    $path = drupal_strtolower(drupal_get_path_alias($url));
    $page_match = drupal_match_path($path, $pages);
    if ($path != $url) {
      $page_match = $page_match || drupal_match_path($url, $pages);
    }
    $page_match = !($visibility xor $page_match);
  }
  elseif (module_exists('php')) {
    $page_match = php_eval($pages);
  }
  else {
    $page_match = FALSE;
  }

  return $page_match;
}
