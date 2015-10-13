<?php
/**
 * @file
 * geocluster_demo.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function geocluster_demo_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:block/geocluster-test-default-json-map
  $menu_links['main-menu:block/geocluster-test-default-json-map'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'block/geocluster-test-default-json-map',
    'router_path' => 'block/%',
    'link_title' => 'Default map',
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
    'weight' => '13',
  );
  // Exported menu link: main-menu:block/geocluster-test-mysql-json-map
  $menu_links['main-menu:block/geocluster-test-mysql-json-map'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'block/geocluster-test-mysql-json-map',
    'router_path' => 'block/%',
    'link_title' => 'MySQL map',
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
    'weight' => '10',
  );
  // Exported menu link: main-menu:block/geocluster-test-php-json-map
  $menu_links['main-menu:block/geocluster-test-php-json-map'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'block/geocluster-test-php-json-map',
    'router_path' => 'block/%',
    'link_title' => 'PHP map',
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
    'weight' => '11',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Default map');
  t('MySQL map');
  t('PHP map');


  return $menu_links;
}
