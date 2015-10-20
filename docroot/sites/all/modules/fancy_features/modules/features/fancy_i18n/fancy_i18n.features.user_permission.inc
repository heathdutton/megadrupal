<?php
/**
 * @file
 * fancy_i18n.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fancy_i18n_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create fancy_page_i18n content'.
  $permissions['create fancy_page_i18n content'] = array(
    'name' => 'create fancy_page_i18n content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any fancy_page_i18n content'.
  $permissions['delete any fancy_page_i18n content'] = array(
    'name' => 'delete any fancy_page_i18n content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own fancy_page_i18n content'.
  $permissions['delete own fancy_page_i18n content'] = array(
    'name' => 'delete own fancy_page_i18n content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'eck add content_i18n bundles'.
  $permissions['eck add content_i18n bundles'] = array(
    'name' => 'eck add content_i18n bundles',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck add content_i18n text_i18n entities'.
  $permissions['eck add content_i18n text_i18n entities'] = array(
    'name' => 'eck add content_i18n text_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck add fancy_section responsive_i18n entities'.
  $permissions['eck add fancy_section responsive_i18n entities'] = array(
    'name' => 'eck add fancy_section responsive_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck administer content_i18n bundles'.
  $permissions['eck administer content_i18n bundles'] = array(
    'name' => 'eck administer content_i18n bundles',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck administer content_i18n text_i18n entities'.
  $permissions['eck administer content_i18n text_i18n entities'] = array(
    'name' => 'eck administer content_i18n text_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck administer fancy_section responsive_i18n entities'.
  $permissions['eck administer fancy_section responsive_i18n entities'] = array(
    'name' => 'eck administer fancy_section responsive_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck delete content_i18n bundles'.
  $permissions['eck delete content_i18n bundles'] = array(
    'name' => 'eck delete content_i18n bundles',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck delete content_i18n text_i18n entities'.
  $permissions['eck delete content_i18n text_i18n entities'] = array(
    'name' => 'eck delete content_i18n text_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck delete fancy_section responsive_i18n entities'.
  $permissions['eck delete fancy_section responsive_i18n entities'] = array(
    'name' => 'eck delete fancy_section responsive_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck edit content_i18n bundles'.
  $permissions['eck edit content_i18n bundles'] = array(
    'name' => 'eck edit content_i18n bundles',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck edit content_i18n text_i18n entities'.
  $permissions['eck edit content_i18n text_i18n entities'] = array(
    'name' => 'eck edit content_i18n text_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck edit fancy_section responsive_i18n entities'.
  $permissions['eck edit fancy_section responsive_i18n entities'] = array(
    'name' => 'eck edit fancy_section responsive_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck list content_i18n bundles'.
  $permissions['eck list content_i18n bundles'] = array(
    'name' => 'eck list content_i18n bundles',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck list content_i18n text_i18n entities'.
  $permissions['eck list content_i18n text_i18n entities'] = array(
    'name' => 'eck list content_i18n text_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck list fancy_section responsive_i18n entities'.
  $permissions['eck list fancy_section responsive_i18n entities'] = array(
    'name' => 'eck list fancy_section responsive_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck view content_i18n bundles'.
  $permissions['eck view content_i18n bundles'] = array(
    'name' => 'eck view content_i18n bundles',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck view content_i18n text_i18n entities'.
  $permissions['eck view content_i18n text_i18n entities'] = array(
    'name' => 'eck view content_i18n text_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'eck view fancy_section responsive_i18n entities'.
  $permissions['eck view fancy_section responsive_i18n entities'] = array(
    'name' => 'eck view fancy_section responsive_i18n entities',
    'roles' => array(),
    'module' => 'eck',
  );

  // Exported permission: 'edit any fancy_page_i18n content'.
  $permissions['edit any fancy_page_i18n content'] = array(
    'name' => 'edit any fancy_page_i18n content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own fancy_page_i18n content'.
  $permissions['edit own fancy_page_i18n content'] = array(
    'name' => 'edit own fancy_page_i18n content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'manage content_i18n properties'.
  $permissions['manage content_i18n properties'] = array(
    'name' => 'manage content_i18n properties',
    'roles' => array(),
    'module' => 'eck',
  );

  return $permissions;
}
