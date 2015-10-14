<?php
/**
 * @file
 * enterprise_webform.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function enterprise_webform_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:node/1
  $menu_links['main-menu:node/1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/1',
    'router_path' => 'node/%',
    'link_title' => 'Form',
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
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Form');


  return $menu_links;
}
