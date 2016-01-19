<?php
/**
 * @file
 * showoff_misc.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function showoff_misc_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:home
  $menu_links['main-menu:home'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<front>',
    'router_path' => '<front>',
    'link_title' => 'Home',
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
    'weight' => '-50',
  );

  // Exported menu link: main-menu:help
  $menu_links['main-menu:help'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'help',
    'router_path' => 'help',
    'link_title' => 'Help',
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
    'weight' => '-43',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Help');
  t('Home');

  return $menu_links;
}
