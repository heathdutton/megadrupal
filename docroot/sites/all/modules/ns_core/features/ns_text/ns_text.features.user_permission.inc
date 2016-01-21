<?php
/**
 * @file
 * Default permissiosn for ns_text.
 */

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function ns_text_defaultconfig_user_default_permissions() {
  // Exported permission: use text format html_editor
  $permissions['use text format html_editor'] = array(
    'name' => 'use text format html_editor',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
      6 => 'translator'
    ),
    'module' => 'filter',
  );
  return $permissions;
}