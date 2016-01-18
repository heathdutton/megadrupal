<?php

/**
 * @file
 * API functions for the user_diff module.
 * WARNING: Do NOT include this file ANYWHERE. This file is only used to have all
 * API functions in one place for documentation purposed. If this file is included
 * you will get error messages, possibly fatal errors ("Cannot redeclare previously
 * declared function ...)
 *
 * You have been warned.
 */

/**
 * Hooks.
 */

/**
 * Returns a keyed array. Keys are the machine-readable names of the field, they
 * don't really matter, but it is recommended to use them for code-readability.
 * The values are arrays with the following keys:
 *   #name The name to display as the header for this field.
 *   #old    The old value for this field.
 *   #new    The new value for this field.
 *   #weight The weight of this field (used when displaying the diff, optional)
 * @param object $old_user The old user object
 * @param object $new_user The new user object
 */
function hook_user_diff($old_user, $new_user) {
  $result = array();
  $result['name'] = array(
    '#name' => t('User Name'),
    '#old' => array($old_user->name),
    '#new' => array($new_user->name),
    '#weight' => 4,
  );

  $result['email'] = array(
    '#name' => t('User Email'),
    '#old' => array($old_user->mail),
    '#new' => array($new_user->mail),
    '#weight' => 5,
  );

  return $result;
}

/**
 * Functions.
 */
/**
 * Determines whether a user has access to perform a certain operation on a revision.
 * @param object $user The user object of the user in question.
 * @param mixed $op Either a string or an array:
 * String: A single word describing the action: 'view', 'edit', 'delete', 'revert'
 * Array: Each element is a full permission string: 'view user revisions', 'edit user revisions', 'delete user revisions', 'revert user revisions'
 */
function user_diff_user_revision_access($user, $op = "view") {

}

/**
 * Creates an array of rows which represent a diff between $old_user and $new_user.
 * The rows can be used via theme('diff_table') to be displayed.
 *
 * @param $old_user
 *   User for comparison which will be displayed on the left side.
 * @param $new_user
 *   User for comparison which will be displayed on the right side.
 */
function _user_diff_body_rows($old_user, $new_user) {
  
}

/**
 * Helper function to invoke hook_user_diff in all enabled modules that implement it.
 */
function _user_diff_module_invoke_all($old_user, $new_user) {

}

/**
 * Show the inline diff for a given user, vid. If vid = 0 or no previous vid
 * exists for the given revision returns the normally rendered content of the
 * specified revision. If metadata is TRUE a header will be added with a legend
 * explaining the color code. This function returns HTML code
 */
function user_diff_inline_show($user, $vid = 0, $metadata = TRUE) {

}

/**
 * Theme function to display the revisions formular with means to select
 * two revisions. Do not call this function direclty. Use theme('user_diff_user_revisions', $vars);
 */
function theme_user_diff_user_revisions($vars) {

}

/**
 * Display inline diff metadata.
 * @see user_diff_inline_show().
 * Do not call this function direclty. Use theme('user_diff_inline_metadata', $vars);
 */
function theme_user_diff_inline_metadata($vars) {

}