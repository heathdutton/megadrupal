<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - registry.
 */

/**
 * Implements hook_ctools_plugin_type() to inform CTools about the layout plugin.
 */
function _fc_ctools_plugin_type() {
  return array(
    'fields' => array(
      'cache' => TRUE,
      'defaults' => array(
        'field form' => array(),
        'cardinality check' => 'fc_default_field_cardinality',
        'can be empty' => FALSE,
  			'zero can be empty' => FALSE,
      ),
    ),
  );
}

/**
 * Implements hook_menu().
 */
function _fc_menu() {
  return array(
    'admin/config/content/fc' => array(
      'title' => 'Field Complete',
      'description' => t('Settings for the field complete module.'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('fc_settings'),
      'access arguments' => array('administer fc settings'),
      'file' => 'fc.admin.inc',
      'type' => MENU_NORMAL_ITEM,
    ),
    'admin/config/content/fc/settings' => array(
      'title' => 'Settings',
      'description' => t('Settings for the field complete module.'),
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => 0,
    ),
    'admin/config/content/fc/rebuild' => array(
      'title' => 'Rebuild',
      'description' => t('Rebuild the field completeness table.'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('fc_rebuild'),
      'access arguments' => array('administer fc settings'),
      'file' => 'fc.admin.inc',
      'type' => MENU_LOCAL_TASK,
      'weight' => 5,
    ),
  );
}

/**
 * Implements hook_permission().
 */
function _fc_permission() {
  return array(
    'administer fc settings' => array(
      'title' => t('Administer Field Complete settings'),
    ),
  );
}

/**
 * Implements hook_theme().
 */
function _fc_theme() {
  return array(
    'form_complete_marker' => array(
      'render element' => 'element',
      'file' => 'fc.themes.inc',
    ),
  );
}
