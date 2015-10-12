<?php
/**
 * @file
 * mvpcreator_marketing.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function mvpcreator_marketing_defaultconfig_features() {
  return array(
    'mvpcreator_marketing' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function mvpcreator_marketing_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create fieldable mvpcreator_marketing_cta'.
  $permissions['create fieldable mvpcreator_marketing_cta'] = array(
    'name' => 'create fieldable mvpcreator_marketing_cta',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable mvpcreator_marketing_cta'.
  $permissions['delete fieldable mvpcreator_marketing_cta'] = array(
    'name' => 'delete fieldable mvpcreator_marketing_cta',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable mvpcreator_marketing_cta'.
  $permissions['edit fieldable mvpcreator_marketing_cta'] = array(
    'name' => 'edit fieldable mvpcreator_marketing_cta',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  return $permissions;
}
