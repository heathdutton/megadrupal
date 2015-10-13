<?php
/**
 * @file
 * openacademy_people.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function openacademy_people_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:people/all
  $menu_links['main-menu:people/all'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'people/all',
    'router_path' => 'people',
    'link_title' => 'All People',
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
    'parent_path' => 'people',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('All People');


  return $menu_links;
}

