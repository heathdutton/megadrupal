<?php

/**
 * @file
 * Page callbacks for the Magento Product integration module.
 */

/**
 * Page callback for reporting details of a Magento product.
 */
function magento_products_node($node) {
  $magento_details = db_query('SELECT magento_product_id, synced FROM {magento_products} WHERE entity_id = :entity_id AND entity_type = :entity_type AND bundle = :bundle', array(
    ':bundle' => variable_get('magento_product_node_content_type'),
    ':entity_type' => 'node',
    ':entity_id' => $node->nid,
  ))->fetchAssoc();

  return t('<p>This product is linked to Magento product ID @product_id and was last imported on @import_date.</p>', array(
    '@product_id' => $magento_details['magento_product_id'],
    '@import_date' => format_date($magento_details['synced']),
  ));
}
