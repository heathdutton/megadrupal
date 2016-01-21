<?php
/**
 * @file
 * Default permissiosn for ns_markdown.
 */

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function ns_markdown_defaultconfig_user_default_permissions() {
  // Exported permission: use text format markdown
  $permissions['use text format markdown'] = array(
    'name' => 'use text format markdown',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
      6 => 'translator',
    ),
    'module' => 'filter',
  );
  return $permissions;
}
