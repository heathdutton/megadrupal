<?php
/**
 * @file
 * openacademy_core.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function openacademy_core_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-footer-menu.
  $menus['menu-footer-menu'] = array(
    'menu_name' => 'menu-footer-menu',
    'title' => 'Footer Menu',
    'description' => 'The footer menu is used on many sites to provide important links at the bottom of the page.',
  );
  // Exported menu: menu-header-menu.
  $menus['menu-header-menu'] = array(
    'menu_name' => 'menu-header-menu',
    'title' => 'Header Menu',
    'description' => 'The footer menu is used on many sites to provide important links at the bottom of the page.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Footer Menu');
  t('Header Menu');
  t('The footer menu is used on many sites to provide important links at the bottom of the page.');


  return $menus;
}
