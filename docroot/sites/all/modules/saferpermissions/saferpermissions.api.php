<?php

/**
 * @file
 * Hooks provided by the saferpermissions module.
 *
 * The purpose of this module is to disallow site administrators to assign
 * certain permissions to the anonymous user.
 *
 * Please note that the current list of permissions are gardens-specific, if
 * you want to open source this code, you probably want to move some of the
 * permissions to scarecrow or gardens_misc.
 *
 * The module generates a list of permissions which should be banned (see
 * later). Using that list, it hooks into the user_admin_permissions form,
 * disables checkboxes and sets element validate callbacks to make sure that
 * the checkboxes can't be checked even if the form is manipulated with
 * firebug or webkit inspector.
 *
 * Generating the list of disallowed permissions has 4 components.
 * - The first component is a simple list (blacklist) of permissions.
 * - The second component is a list of words. Every permission which contains
 *   the given word will be added to the blacklist.
 *   Example: if the given word is 'car', 'start the car' will be blacklisted,
 *   but 'clean the carpet' will not.
 * - The third component is not a dynamic one. Any permission, which has the
 *   restricted property will be automatically added to the blacklist.
 * - The fourth component is a whitelist. Any permission on this list will not
 *   be added to the final list of permissions even if any of the previous
 *   components mention the permission.
 *
 * @see saferpermissions_disallowed_permissions_anonymous().
 * @see hook_permission().
 */

/**
 * Returns a list of permissions.
 *
 * These permissions will be unavailable for the anonymous user.
 *
 * @return array
 */
function hook_saferpermissions_anonymous_permission_ban_info() {
  return array(
    'access administration pages',
    'access site in maintenance mode',
    'view the administration theme',
    'access site reports',
  );
}

/**
 * Alters the list of blacklisted permissions.
 *
 * @see hook_anonymous_permission_ban_info().
 *
 * @param array $permissions
 */
function hook_saferpermissions_anonymous_permission_ban_info_alter(array &$permissions) {
  // remove items here
}

/**
 * Returns a list of permissions.
 *
 * These permissions will be available for the anonymous user.
 *
 * @return array
 */
function hook_saferpermissions_anonymous_permission_whitelist_info() {
  return array(
    'cancel own vote',
    'inspect all votes',
  );
}

/**
 * Alters the list of whitelisted permissions.
 *
 * @see hook_anonymous_permission_whitelist_info().
 *
 * @param array $permissions
 */
function hook_saferpermissions_anonymous_permission_whitelist_info_alter(array &$permissions) {
  // remove items here
}

/**
 * Returns a list of words.
 *
 * Any permission containing a word from the list will be blacklisted.
 *
 * @return array
 */
function hook_saferpermissions_anonymous_permission_wordlist_ban_info() {
  return array(
    'administer',
    'bypass',
  );
}

/**
 * Alters the list of words.
 *
 * @see hook_anonymous_permission_wordlist_ban_info().
 *
 * @param array $words
 */
function hook_saferpermissions_anonymous_permission_wordlist_ban_info_alter(array &$words) {
  // remove items here
}
