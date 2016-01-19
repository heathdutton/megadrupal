<?php
/**
 * @file
 * showoff_video.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function showoff_video_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:node/add/video
  $menu_links['main-menu:node/add/video'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/video',
    'router_path' => 'node/add/video',
    'link_title' => 'Add Video',
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
    'weight' => '-46',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Add Video');


  return $menu_links;
}
