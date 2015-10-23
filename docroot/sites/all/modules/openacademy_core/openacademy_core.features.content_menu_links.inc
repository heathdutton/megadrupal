<?php
/**
 * @file
 * openacademy_core.features.content_menu_links.inc
 */

/**
 * Implements hook_content_menu_links_defaults().
 */
function openacademy_core_content_menu_links_defaults() {
  $menu_links = array();

  // Exported menu link: menu-header-menu:user/login
  $menu_links['menu-header-menu:user/login'] = array(
    'menu_name' => 'menu-header-menu',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Log in',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 40,
    'customized' => 0,
  );
  // Exported menu link: menu-header-menu:user/logout
  $menu_links['menu-header-menu:user/logout'] = array(
    'menu_name' => 'menu-header-menu',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Log out',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 30,
    'customized' => 0,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Log in');
  t('Log out');


  return $menu_links;
}
