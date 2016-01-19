<?php
/**
 * @file
 * openlucius_news.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function openlucius_news_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create ol_news content'.
  $permissions['create ol_news content'] = array(
    'name' => 'create ol_news content',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
      'client' => 'client',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any ol_news content'.
  $permissions['delete any ol_news content'] = array(
    'name' => 'delete any ol_news content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own ol_news content'.
  $permissions['delete own ol_news content'] = array(
    'name' => 'delete own ol_news content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any ol_news content'.
  $permissions['edit any ol_news content'] = array(
    'name' => 'edit any ol_news content',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own ol_news content'.
  $permissions['edit own ol_news content'] = array(
    'name' => 'edit own ol_news content',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
      'client' => 'client',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'promote news'.
  $permissions['promote news'] = array(
    'name' => 'promote news',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'openlucius_news',
  );

  // Exported permission: 'view comment alterations in ol_news'.
  $permissions['view comment alterations in ol_news'] = array(
    'name' => 'view comment alterations in ol_news',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
      'client' => 'client',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'comment_alter',
  );

  return $permissions;
}
