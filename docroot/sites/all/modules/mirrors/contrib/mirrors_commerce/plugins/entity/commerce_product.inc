<?php

/**
 * @file
 * Mirrors entity plugin declaration.
 */

/**
 * Entity Declaration: Commerce Product.
 */
$plugin = array(
  'title' => t('Commerce Product'),
  'category' => 'Entity',
  'views' => array(
    'base_table' => 'commerce_product',
  ),
  'feeds' => array(
    'processor' => 'FeedsCommerceProductProcessor',
  ),
  'properties' => array(
    'type' => array(
      'type' => 'text',
    ),
    'status' => array(
      'type' => 'boolean',
    ),
    'product_id' => array(
      'type' => 'integer',
    ),
    'sku' => array(
      'type' => 'text',
    ),
    'title' => array(
      'type' => 'text',
    ),
  ),
);
