<?php
/**
 * @file
 * fancy_content_rich_text.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fancy_content_rich_text_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'eck add fancy_content rich_text entities'.
  $permissions['eck add fancy_content rich_text entities'] = array(
    'name' => 'eck add fancy_content rich_text entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck administer fancy_content rich_text entities'.
  $permissions['eck administer fancy_content rich_text entities'] = array(
    'name' => 'eck administer fancy_content rich_text entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck delete fancy_content rich_text entities'.
  $permissions['eck delete fancy_content rich_text entities'] = array(
    'name' => 'eck delete fancy_content rich_text entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck edit fancy_content rich_text entities'.
  $permissions['eck edit fancy_content rich_text entities'] = array(
    'name' => 'eck edit fancy_content rich_text entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck list fancy_content rich_text entities'.
  $permissions['eck list fancy_content rich_text entities'] = array(
    'name' => 'eck list fancy_content rich_text entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'eck view fancy_content rich_text entities'.
  $permissions['eck view fancy_content rich_text entities'] = array(
    'name' => 'eck view fancy_content rich_text entities',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'eck',
  );

  // Exported permission: 'use text format wysiwyg'.
  $permissions['use text format wysiwyg'] = array(
    'name' => 'use text format wysiwyg',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
