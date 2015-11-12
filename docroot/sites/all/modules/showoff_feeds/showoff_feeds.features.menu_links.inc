<?php
/**
 * @file
 * showoff_feeds.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function showoff_feeds_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:feeds
  $menu_links['main-menu:feeds'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'feeds',
    'router_path' => 'feeds',
    'link_title' => 'Feeds',
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
    'weight' => '-49',
  );

  // Exported menu link: main-menu:current
  $menu_links['main-menu:current'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'current',
    'router_path' => 'current',
    'link_title' => 'Current',
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
    'weight' => '-45',
  );

  // Exported menu link: main-menu:expired
  $menu_links['main-menu:expired'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'expired',
    'router_path' => 'expired',
    'link_title' => 'Expired',
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
    'weight' => '-44',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Current');
  t('Expired');
  t('Feeds');

  return $menu_links;
}
