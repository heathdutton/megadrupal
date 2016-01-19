<?php
/**
 * @file
 * Contains the $_callback, $_access, and $_cache_id functions for this block.
 */

/**
 * Whatever is returned by this anonymous function will get passed to the tpl.
 *
 * @param object $o_entity
 *   The entity object of the term or node that this block appears on.
 *
 * @return array
 *   An array of elements to pass to the template defined in this directory.
 */
$_callback = function () use (&$o_entity) {
  $array_to_return = array();
  // Whatever this callback function returns, will be available in the template.
  // Best practice is to return an array or object so that you can pass multiple
  // meaningful items to your tpl.
  // Here is where your code should start.
  return $array_to_return;
};

/**
 * If this returns FALSE, the block will be INaccessible to the user.
 *
 * To limit access based on anonymous or logged-in, use user_is_logged_in():
 *   https://api.drupal.org/api/drupal/modules%21user%21user.module/function/user_is_logged_in/7
 * To limit access based on permission, use user_access():
 *   https://api.drupal.org/api/drupal/modules%21user%21user.module/function/user_access/7
 * To limit access based on a user's roles, use user_roles()
 *   https://api.drupal.org/api/drupal/modules%21user%21user.module/function/user_roles/7
 *
 * @param object $o_entity
 *   The entity object of the term or node that this block appears on.
 *
 * @return bool
 *   - TRUE (or any non-false) will cause the block to be rendered.
 *   - FALSE will prevent the block from being rendered.
 */
$_access = function ($user = '') use (&$o_entity) {
  // Do any kind of evaluation you need here.  return TRUE if you want the block
  // visible, return FALSE if you want it invisible.
  return TRUE;
};

/**
 * Return a unique ID if you would like this block cached.
 *
 * @param object $o_entity
 *   The entity object of the term or node that this block appears on.
 *
 * @return string
 *   The unique cache id. Adding a pipe (optional) followed by an integer of the
 *   number of seconds will specify  the minimum duration in seconds, it will
 *   remained cached.
 *   - If the block should be cached per page, use the url like this.
 *     Example: return current_path();
 *   - If it should be cached per user, use a uid plus something
 *     Example: return {$user->uid}-{$delta};
 *   - If it should be cached across the whole site, use the block delta.
 *     Example: $return $delta;
 *   - If it should NOT be cached, return FALSE
 */
$_cache_id = function ($user) use (&$o_entity, $delta) {
  // Build your unique cache_id to return here.  Return FALSE if it
  // should not have extended caching.  To control how long this item should be
  // cached, pipe in the number of seconds this should be cached 3600.
  return FALSE;
};

// You can place other functions here that will only be available if this block
// is placed.  Name them carefully to prevent fatal naming collisions.
// Example: codit_blocks_BLOCKDELTA_FUNCTION_NAME().
