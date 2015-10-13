<?php

/**
 * @file
 * This file contains the hooks that can be used by other modules.
 */

/**
 * This hook gets invoked when HTML element marked as read.
 *
 * @param int $list_id
 *   Id of the List that is getting inserted.
 * @param string $attribute_value
 *   Unique value of the attribute that's going to be inserted.
 */
function hook_mark_as_read_marked_read($list_id, $attribute_value) {
  global $user;
  // Save into custom audit logs.
  db_save_into_audit($user->uid, REQUEST_TIME, $attribute_value);
}

/**
 * Alters the list data.
 *
 * This hook contains the array of lists added on the admin, based on which
 * javascript iterates the DOM element.
 *
 * This needs to be called, if that list array  needs to be modified.
 *
 * @param array $list
 *   Associative array of lists.
 */
function hook_mark_as_read_list_alter(array &$list) {
  global $user;
  if (request_uri() == 'user/test' && in_array('editor', $user->roles)) {
    unset($list['20']);
  }
}

/**
 * It prevents Javascript from loading.
 *
 * Precedence will be given to this hook over other conditions.
 *
 * @return bool
 *   Returning TRUE won't load Javascript file at all and
 *   prevents 'mark_as_read' behaviour.
 */
function hook_mark_as_read_prevent() {
  global $user;
  // Do not load Javascript if this condition is satisfied.
  if (request_uri() == 'user/test' && in_array('editor', $user->roles)) {
    return TRUE;
  }
  return FALSE;
}
