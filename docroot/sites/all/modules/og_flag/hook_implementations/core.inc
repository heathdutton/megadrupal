<?php
/**
 * @file
 * Contains all implementations of Drupal core hooks.
 */

/**
 * Implements hook_menu().
 */
function og_flag_menu() {
  $items = array();

  $items['group/%/%/admin/og-flag'] = array(
    'title' => 'Group Flag Settings',
    'page callback' => array('drupal_get_form'),
    'access arguments' => array('administer group'),
    // Menu Param1 is group type.
    // Menu Param2 is group id.
    'page arguments' => array('og_flag_admin_settings_listing', 1, 2),
    'type' => MENU_CALLBACK,
    'file' => 'og_flag.admin.inc',
  );

  $items['group/%/%/admin/og-flag/%/edit'] = array(
    'title' => 'Edit Group Flag Settings',
    'page callback' => array('drupal_get_form'),
    'access arguments' => array('administer group'),
    // Menu Param1 is group type.
    // Menu Param2 is group id.
    // Menu Param 3 is flag id.
    'page arguments' => array('og_flag_admin_settings_edit', 1, 2, 5),
    'type' => MENU_CALLBACK,
    'file' => 'og_flag.admin.inc',
  );

  return $items;
}
