<?php
/**
 * @file
 * fancy_content_map.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fancy_content_map_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'eck add fancy_content map entities'.
  $permissions['eck add fancy_content map entities'] = array(
    'name' => 'eck add fancy_content map entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck administer fancy_content map entities'.
  $permissions['eck administer fancy_content map entities'] = array(
    'name' => 'eck administer fancy_content map entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck delete fancy_content map entities'.
  $permissions['eck delete fancy_content map entities'] = array(
    'name' => 'eck delete fancy_content map entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck edit fancy_content map entities'.
  $permissions['eck edit fancy_content map entities'] = array(
    'name' => 'eck edit fancy_content map entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck list fancy_content map entities'.
  $permissions['eck list fancy_content map entities'] = array(
    'name' => 'eck list fancy_content map entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck view fancy_content map entities'.
  $permissions['eck view fancy_content map entities'] = array(
    'name' => 'eck view fancy_content map entities',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'eck',
  );

  return $permissions;
}
