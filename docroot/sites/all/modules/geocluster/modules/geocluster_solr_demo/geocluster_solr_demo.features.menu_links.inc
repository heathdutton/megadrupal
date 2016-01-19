<?php
/**
 * @file
 * geocluster_solr_demo.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function geocluster_solr_demo_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:block/geocluster-test-solr-json-map
  $menu_links['main-menu:block/geocluster-test-solr-json-map'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'block/geocluster-test-solr-json-map',
    'router_path' => 'block/%',
    'link_title' => 'Solr map',
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
    'weight' => '12',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Solr map');


  return $menu_links;
}
