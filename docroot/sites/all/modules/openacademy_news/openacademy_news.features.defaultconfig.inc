<?php
/**
 * @file
 * openacademy_news.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function openacademy_news_defaultconfig_features() {
  return array(
    'openacademy_news' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function openacademy_news_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create openacademy_news content'.
  $permissions['create openacademy_news content'] = array(
    'name' => 'create openacademy_news content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any openacademy_news content'.
  $permissions['delete any openacademy_news content'] = array(
    'name' => 'delete any openacademy_news content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own openacademy_news content'.
  $permissions['delete own openacademy_news content'] = array(
    'name' => 'delete own openacademy_news content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any openacademy_news content'.
  $permissions['edit any openacademy_news content'] = array(
    'name' => 'edit any openacademy_news content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own openacademy_news content'.
  $permissions['edit own openacademy_news content'] = array(
    'name' => 'edit own openacademy_news content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
