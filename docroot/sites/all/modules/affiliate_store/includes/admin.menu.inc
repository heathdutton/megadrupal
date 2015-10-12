<?php

/**
 * @file
 * Menu path for admin pages.
 */

/**
 * Implements menu passthrough hook.
 */
function _affiliate_store_admin_menu(&$items) {
  $items['admin/structure/affiliate-store'] = array(
    'title' => 'Affiliate store',
    'description' => 'Configure affiliate store settings.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('affiliate_store_settings_page'),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
  );
  $items['admin/structure/affiliate-store/settings'] = array(
    'title' => 'Settings',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  $items['admin/structure/affiliate-store/settings/basic'] = array(
    'title' => 'Basic',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  $items['admin/structure/affiliate-store/settings/advanced'] = array(
    'title' => 'Advanced',
    'description' => 'Configure affiliate store advanced settings.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('affiliate_store_advanced_settings_page'),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/structure/affiliate-store/subscriptions'] = array(
    'title' => 'Subscriptions',
    'description' => 'Manage affiliate store subscriptions.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('affiliate_store_subscription_page'),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/structure/affiliate-store/update'] = array(
    'title' => 'Manual update',
    'description' => 'Update affiliate store manually.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('affiliate_store_update_page'),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/structure/affiliate-store/map'] = array(
    'title' => 'Mapping',
    'description' => 'Rename and merge product categories.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('affiliate_store_category_map_page', 'category'),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_LOCAL_TASK,
  );
  $items['admin/structure/affiliate-store/map/category'] = array(
    'title' => 'Category',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );
  $items['admin/structure/affiliate-store/map/subcategory'] = array(
    'title' => 'Subcategory',
    'description' => 'Rename and merge product subcategories.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'affiliate_store_category_map_page', 'subcategory',
    ),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_LOCAL_TASK,
  );
  $items['admin/structure/affiliate-store/map/subsubcategory'] = array(
    'title' => 'Subsubcategory',
    'description' => 'Rename and merge product subsubcategories.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array(
      'affiliate_store_category_map_page', 'subsubcategory',
    ),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_LOCAL_TASK,
  );
  $items['admin/structure/affiliate-store/map/term/%/edit'] = array(
    'title' => 'Edit term synonym',
    'description' => 'Edit term synonym.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('affiliate_store_category_map_edit_page', 5),
    'access callback' => 'user_access',
    'access arguments' => array('administer affiliate store'),
    'file' => 'includes/admin.inc',
    'type' => MENU_CALLBACK,
  );
}
