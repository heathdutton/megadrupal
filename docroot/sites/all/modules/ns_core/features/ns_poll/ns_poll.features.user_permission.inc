<?php
/**
 * @file
 * ns_poll.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_poll_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: cancel own vote
  $permissions['cancel own vote'] = array(
    'name' => 'cancel own vote',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'super user',
    ),
    'module' => 'poll',
  );

  // Exported permission: create poll content
  $permissions['create poll content'] = array(
    'name' => 'create poll content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any poll content
  $permissions['delete any poll content'] = array(
    'name' => 'delete any poll content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own poll content
  $permissions['delete own poll content'] = array(
    'name' => 'delete own poll content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any poll content
  $permissions['edit any poll content'] = array(
    'name' => 'edit any poll content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own poll content
  $permissions['edit own poll content'] = array(
    'name' => 'edit own poll content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer',
    ),
    'module' => 'node',
  );

  // Exported permission: inspect all votes
  $permissions['inspect all votes'] = array(
    'name' => 'inspect all votes',
    'roles' => array(
      0 => 'administrator',
      1 => 'super user',
    ),
    'module' => 'poll',
  );

  // Exported permission: vote on polls
  $permissions['vote on polls'] = array(
    'name' => 'vote on polls',
    'roles' => array(
      0 => 'authenticated user',
      1 => 'chief editor',
    ),
    'module' => 'poll',
  );

  return $permissions;
}
