<?php
/**
 * @file
 * multi_currency_pricelist.features.inc
 */

/**
 * Implements hook_views_api().
 */
function multi_currency_pricelist_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_fe_nodequeue_export_fields().
 */
function multi_currency_pricelist_fe_nodequeue_export_fields() {
  $nodequeues = array();

  // Exported nodequeues: pricelist_order_items
  $nodequeues['pricelist_order_items'] = array(
    'name' => 'pricelist_order_items',
    'title' => 'Pricelist order items',
    'subqueue_title' => '',
    'size' => 0,
    'link' => '',
    'link_remove' => '',
    'owner' => 'smartqueue_taxonomy',
    'show_in_ui' => 1,
    'show_in_tab' => 1,
    'show_in_links' => 0,
    'reference' => 'field_pricelist_section',
    'reverse' => 0,
    'i18n' => 0,
    'types' => array(
      0 => 'pricelist_item',
    ),
    'roles' => array(),
    'count' => 0,
    'use_parents' => 0,
  );

  return $nodequeues;
}

/**
 * Implements hook_node_info().
 */
function multi_currency_pricelist_node_info() {
  $items = array(
    'pricelist_item' => array(
      'name' => t('Pricelist item'),
      'base' => 'node_content',
      'description' => t('A item with a price in a base currency to be included on a multi-currency pricelist.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
