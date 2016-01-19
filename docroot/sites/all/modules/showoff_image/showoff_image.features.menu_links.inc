<?php
/**
 * @file
 * showoff_image.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function showoff_image_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:node/add/image
  $menu_links['main-menu:node/add/image'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/image',
    'router_path' => 'node/add/image',
    'link_title' => 'Add Image',
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
    'weight' => -47,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Add Image');


  return $menu_links;
}
