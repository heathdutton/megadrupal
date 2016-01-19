<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: commerce product reference.
 */
$plugin = array(
  'title' => t('Commerce Product Reference'),
  'category' => 'Field',
  'views' => array(
    'type' => 'commerce_product_reference_sku_plain',
    'settings' => array(
      'show_quantity' => '0',
      'default_quantity' => '1',
      'combine' => '1',
      'show_single_product_attributes' => '0',
      'line_item_type' => '0',
    ),
  ),
  'feeds' => array(
    'target' => 'SELF:sku',
  ),
);
