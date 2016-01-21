<?php
/**
 * @file
 * bassets_server_user.features.user_role_plus.inc
 */

/**
 * Implements hook_user_default_roles_plus().
 */
function bassets_server_user_user_default_roles_plus() {
  $roles = array();

  // Exported role: Bassets Connection
  $roles['Bassets Connection'] = array(
    'name' => 'Bassets Connection',
    'weight' => 3,
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content' => TRUE,
      'access setup' => TRUE,
      'access_bassets_file_usage_page' => TRUE,
      'administer services client' => TRUE,
      'administer services client connection' => TRUE,
      'deploy_file_to_client' => TRUE,
      'oauth authorize any consumers' => TRUE,
      'oauth register any consumers' => TRUE,
      'search content' => TRUE,
      'Use image image admin replacement' => TRUE,
    ),
  );

  // Exported role: Bassets admin
  $roles['Bassets admin'] = array(
    'name' => 'Bassets admin',
    'weight' => 4,
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content overview' => TRUE,
      'access_bassets_file_usage_page' => TRUE,
      'administer file types' => TRUE,
      'administer files' => TRUE,
      'administer image styles' => TRUE,
      'administer nodes' => TRUE,
      'administer taxonomy' => TRUE,
      'bypass file access' => TRUE,
      'create files' => TRUE,
      'delete any audio files' => TRUE,
      'delete any document files' => TRUE,
      'delete any image files' => TRUE,
      'delete any video files' => TRUE,
      'delete own audio files' => TRUE,
      'delete own document files' => TRUE,
      'delete own image files' => TRUE,
      'delete own video files' => TRUE,
      'delete terms in 1' => TRUE,
      'delete terms in 2' => TRUE,
      'delete terms in 3' => TRUE,
      'delete terms in 4' => TRUE,
      'deploy_file_to_client' => TRUE,
      'download own audio files' => TRUE,
      'download own document files' => TRUE,
      'download own image files' => TRUE,
      'download own video files' => TRUE,
      'edit any audio files' => TRUE,
      'edit any document files' => TRUE,
      'edit any image files' => TRUE,
      'edit any video files' => TRUE,
      'edit own audio files' => TRUE,
      'edit own document files' => TRUE,
      'edit own image files' => TRUE,
      'edit own video files' => TRUE,
      'edit terms in 1' => TRUE,
      'edit terms in 2' => TRUE,
      'edit terms in 3' => TRUE,
      'edit terms in 4' => TRUE,
      'flush caches' => TRUE,
      'Use image image admin replacement' => TRUE,
      'view files' => TRUE,
      'view own files' => TRUE,
      'view own private files' => TRUE,
    ),
  );

  // Exported role: Bassets editor
  $roles['Bassets editor'] = array(
    'name' => 'Bassets editor',
    'weight' => 5,
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content overview' => TRUE,
      'access_bassets_file_usage_page' => TRUE,
      'administer nodes' => TRUE,
      'create files' => TRUE,
      'delete own audio files' => TRUE,
      'delete own document files' => TRUE,
      'delete own image files' => TRUE,
      'delete own video files' => TRUE,
      'download any image files' => TRUE,
      'edit own audio files' => TRUE,
      'edit own document files' => TRUE,
      'edit own image files' => TRUE,
      'edit own video files' => TRUE,
      'edit terms in 3' => TRUE,
      'flush caches' => TRUE,
      'Use image image admin replacement' => TRUE,
      'view files' => TRUE,
      'view own files' => TRUE,
      'view own private files' => TRUE,
    ),
  );

  return $roles;
}
