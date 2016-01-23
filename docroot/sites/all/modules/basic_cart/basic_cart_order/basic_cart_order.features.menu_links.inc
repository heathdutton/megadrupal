<?php
/**
 * @file
 * basic_cart_order.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function basic_cart_order_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: management:admin/structure/orders
  $menu_links['management:admin/structure/orders'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/structure/orders',
    'router_path' => 'admin/structure/orders',
    'link_title' => 'Orders',
    'options' => array(
      'attributes' => array(
        'title' => 'View the basic cart orders.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'admin/structure',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Orders');


  return $menu_links;
}
