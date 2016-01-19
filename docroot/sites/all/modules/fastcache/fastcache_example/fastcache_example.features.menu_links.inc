<?php
/**
 * @file
 * fastcache_example.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function fastcache_example_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_favorite-articles:favorite-articles
  $menu_links['main-menu_favorite-articles:favorite-articles'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'favorite-articles',
    'router_path' => 'favorite-articles',
    'link_title' => 'Favorite articles',
    'options' => array(
      'identifier' => 'main-menu_favorite-articles:favorite-articles',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Exported menu link: main-menu_followed-users:followed-users
  $menu_links['main-menu_followed-users:followed-users'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'followed-users',
    'router_path' => 'followed-users',
    'link_title' => 'Followed users',
    'options' => array(
      'identifier' => 'main-menu_followed-users:followed-users',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Exported menu link: main-menu_latest-contents:latest-contents
  $menu_links['main-menu_latest-contents:latest-contents'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'latest-contents',
    'router_path' => 'latest-contents',
    'link_title' => 'Latest contents',
    'options' => array(
      'identifier' => 'main-menu_latest-contents:latest-contents',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Exported menu link: main-menu_latest-users:latest-users
  $menu_links['main-menu_latest-users:latest-users'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'latest-users',
    'router_path' => 'latest-users',
    'link_title' => 'Latest users',
    'options' => array(
      'identifier' => 'main-menu_latest-users:latest-users',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Favorite articles');
  t('Followed users');
  t('Latest contents');
  t('Latest users');


  return $menu_links;
}
