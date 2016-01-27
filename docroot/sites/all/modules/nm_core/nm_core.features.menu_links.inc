<?php
/**
 * @file
 * nm_core.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function nm_core_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-footer-links:contact
  $menu_links['menu-footer-links:contact'] = array(
    'menu_name' => 'menu-footer-links',
    'link_path' => 'contact',
    'router_path' => 'contact',
    'link_title' => 'Contact',
    'options' => array(
      'attributes' => array(
        'title' => 'Contact us',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Contact');


  return $menu_links;
}
