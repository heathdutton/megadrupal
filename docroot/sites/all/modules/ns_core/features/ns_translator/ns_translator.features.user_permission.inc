<?php
/**
 * @file
 * ns_translator.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ns_translator_user_default_permissions() {
  $permissions = array();

  // Exported permission: translate any entity
  $permissions['translate any entity'] = array(
    'name' => 'translate any entity',
    'roles' => array(
      0 => 'translator',
    ),
    'module' => 'entity_translation',
  );

  return $permissions;
}
