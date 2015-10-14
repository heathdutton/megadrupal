<?php

/**
 * Registry function for hook_menu().
 */
function _hansel_ui_menu() {
  $menu = array();

  $menu['admin/config/search/hansel'] = array(
    'title' => 'Hansel',
    'description' => 'Create custom breadcrumbs by user defined rules',
    'page callback' => 'hansel_ui_page',
    'access arguments' => array('administer hansel'),
    'type' => MENU_NORMAL_ITEM,
  );
  $menu['admin/config/search/hansel/configuration'] = array(
    'title' => 'Configuration',
    'page callback' => 'hansel_ui_page',
    'access arguments' => array('administer hansel'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
  );
  $menu['admin/config/search/hansel/configuration/rules'] = array(
    'title' => 'Rules',
    'page callback' => 'hansel_ui_page',
    'access arguments' => array('administer hansel'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
  );
  $menu['admin/config/search/hansel/configuration/nodetypes'] = array(
    'title' => 'Nodetypes',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hansel_ui_nodetypes_form'),
    'access arguments' => array('administer hansel'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
  );
  $menu['admin/config/search/hansel/test'] = array(
    'title' => 'Test',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hansel_ui_test_form'),
    'access arguments' => array('test hansel'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'hansel_ui.test.inc',
  );
  $menu['admin/config/search/hansel/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hansel_ui_settings'),
    'access arguments' => array('administer hansel'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
  );
  $menu['admin/config/search/hansel/rule/%'] = array(
    'title' => 'Edit rule',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hansel_ui_rule_form', 5),
    'access arguments' => array('administer hansel'),
    'type' => MENU_CALLBACK,
  );
  $menu['admin/config/search/hansel/rule/%/delete'] = array(
    'title' => 'Delete rule',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hansel_ui_delete_rule_form', 5),
    'access arguments' => array('administer hansel'),
    'type' => MENU_CALLBACK,
  );
  $menu['admin/config/search/hansel/switch/%'] = array(
    'title' => 'Configure switch',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hansel_ui_config_switch_form', 5),
    'access arguments' => array('administer hansel'),
    'type' => MENU_CALLBACK,
  );
  
  return $menu;
}