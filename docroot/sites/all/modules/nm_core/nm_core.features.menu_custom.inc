<?php
/**
 * @file
 * nm_core.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function nm_core_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-footer-links.
  $menus['menu-footer-links'] = array(
    'menu_name' => 'menu-footer-links',
    'title' => 'Footer Links',
    'description' => 'Links in the footer',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Footer Links');
  t('Links in the footer');


  return $menus;
}
