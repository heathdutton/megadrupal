<?php
/**
 * @file
 * showoff_unit.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function showoff_unit_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:units
  $menu_links['main-menu:units'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'units',
    'router_path' => 'units',
    'link_title' => 'Units',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Units');

  return $menu_links;
}
