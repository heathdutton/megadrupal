<?php

/**
 * Implements hook_menu().
 */
function muchomenu_menu() {
  $items = array();
  $admin_access = array('administer muchomenu');


  $items['admin/structure/menu/muchomenu'] = array(
      'title' => 'MuchoMenus',
      'page callback' => 'muchomenu_overview_page',
      'access arguments' => $admin_access,
      'file' => 'includes/admin.inc',
      'type' => MENU_LOCAL_TASK,
      'weight' => -1,
  );
  $items['admin/structure/menu/muchomenu/list'] = array(
      'title' => 'List menus',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'file' => 'includes/admin.inc',
      'access arguments' => $admin_access,
  );
  $items['admin/structure/menu/muchomenu/add'] = array(
      'title' => 'Add menu',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_edit_menu', 'add'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'includes/admin.inc',
      'access arguments' => $admin_access,
  );
  $items['admin/structure/menu/muchomenu/settings'] = array(
      'title' => 'Settings',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_configure'),
      'type' => MENU_LOCAL_TASK,
      'weight' => 5,
      'file' => 'includes/admin.inc',
      'access arguments' => $admin_access,
  );

  $items['admin/structure/menu/muchomenu/muchomenu-customize/%muchomenu'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_overview_form', 5),
      'access arguments' => $admin_access,
      'file' => 'includes/admin.inc',
      'title callback' => 'muchomenu_overview_title',
      'title arguments' => array(5),
  );


  $items['admin/structure/menu/muchomenu/muchomenu-customize/%muchomenu/list'] = array(
      'title' => 'List items',
      'weight' => -10,
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'file' => 'includes/admin.inc',
  );
  $items['admin/structure/menu/muchomenu/muchomenu-customize/%muchomenu/add'] = array(
      'title' => 'Add item',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_edit_item', 'add', NULL, 5),
      'type' => MENU_LOCAL_TASK,
      'file' => 'includes/admin.inc',
      'access arguments' => $admin_access,
  );
  $items['admin/structure/menu/muchomenu/muchomenu-customize/%muchomenu/edit'] = array(
      'title' => 'Edit menu',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_edit_menu', 'edit', 5),
      'type' => MENU_LOCAL_TASK,
      'file' => 'includes/admin.inc',
      'access arguments' => $admin_access,
  );
  $items['admin/structure/menu/muchomenu/muchomenu-customize/%muchomenu/delete'] = array(
      'title' => 'Delete menu',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_delete_menu_confirm', 5),
      'type' => MENU_CALLBACK,
      'file' => 'includes/admin.inc',
      'access arguments' => $admin_access,
  );


  $items['admin/structure/menu/muchomenu/item/%muchomenu_link/edit'] = array(
      'title' => 'Edit menu item',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_edit_item', 'edit', 5, NULL),
      'type' => MENU_CALLBACK,
      'access arguments' => $admin_access,
      'file' => 'includes/admin.inc',
  );
  $items['admin/structure/menu/muchomenu/item/%muchomenu_link/delete'] = array(
      'title' => 'Delete menu item',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('muchomenu_item_delete_form', 5),
      'type' => MENU_CALLBACK,
      'access arguments' => $admin_access,
      'file' => 'includes/admin.inc',
  );

  return $items;
}

/**
 * Implements hook_permission().
 */
function muchomenu_permission() {
  return array(
      'administer muchomenu' => array(
          'title' => t('administer muchomenu'),
          'description' => t('TODO Add a description for \'administer muchomenu\''),
      ),
  );
}

/**
 * Implements hook_block_info().
 */
function muchomenu_block_info() {
  $menus = muchomenu_get_menus();
  $blocks = array();
  if (!empty($menus)) {
    foreach ($menus as $id => $title) {
      $blocks[$id]['info'] = check_plain($title);
      $blocks[$id]['cache'] = DRUPAL_NO_CACHE;
    }
  }
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function muchomenu_block_view($delta) {
  // $delta represents the menu id
  $menu = muchomenu_load($delta);
  $menu_tree = muchomenu_tree_data($delta, FALSE);
  //$data['subject'] = check_plain($menus[$delta]);
  $data['content'] = theme('muchomenu_menu_tree', array('menu' => $menu, 'tree' => $menu_tree));
  return $data;
}

/**
 * Implements hook_theme().
 */
function muchomenu_theme() {
  return array(
      'muchomenu_overview_form' => array(
          'render element' => 'form',
          'template' => 'templates/muchomenu-overview-form',
          'file' => 'includes/theme.inc',
      ),
      'muchomenu_item' => array(
          'variables' => array('item', 'has_children', 'prefix' => '', 'layout' => 'vertical', 'menu' => ''),
          'file' => 'includes/theme.inc',
      ),
      'muchomenu_tree' => array(
          'variables' => array('tree' => NULL),
          'file' => 'includes/theme.inc',
      ),
      'muchomenu_menu_tree' => array(
          'variables' => array('menu' => NULL, 'tree' => array()),
          'file' => 'includes/theme.inc',
      ),
  );
}
