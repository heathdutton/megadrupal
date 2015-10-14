<?php

/**
 * Drush integration for the Drupal Magento Products integration module.
 */

/**
 * Implements hook_drush_command().
 */
function magento_products_drush_command() {
  $items = array();

  $items['magento-import-products'] = array(
    'description' => dt('Updates the queue with newly added or updated products and processes the queue.'),
    'aliases' => array('magento-import', 'mi'),
  );

  $items['magento-process-queue'] = array(
    'description' => dt('Proceses Magento products in the import queue.'),
    'aliases' => array('magento-import', 'mpq'),
  );

  $items['magento-import-product'] = array(
    'description' => dt("Import a single product into a new node or update it's existing node."),
    'arguments' => array('magento_product_id' => 'Magento product id'),
    'aliases' => array('mip'),
    'required-arguments' => TRUE,
  );

  return $items;
}

/**
 * Drush command callback for importing a single product into Drupal.
 */
function drush_magento_products_magento_import_product($product_id) {
  module_load_include('inc', 'magento_products');
  module_load_include('class.inc', 'magento_products');

  $magento_connection = new MagentoConnection();
  $magento = new MagentoProducts($magento_connection);

  $created = 0;
  $updated = 0;
  $products = $magento->getProducts(array($product_id));

  $node = magento_products_import_product($products[$product_id], $created, $updated);
  if ($updated) {
    $message = dt('Imported product and updated node @title (@nid).', array(
      '@title' => $node->title,
      '@nid' => $node->nid,
    ));
    drush_log($message, 'ok');
  }
  if ($created) {
    $message = dt('Imported product and created node @title (@nid).', array(
      '@title' => $node->title,
      '@nid' => $node->nid,
    ));
    drush_log($message, 'ok');
  }
  if (empty($node)) {
    drush_log(dt('Could not import product. Check the logs for more information.'), 'error');
  }
}

/**
 * Drush command callback for importing updated or new products into Drupal.
 */
function drush_magento_products_magento_import_products() {
  module_load_include('inc', 'magento_products');
  $imported_count = magento_products_import();

  if ($imported_count) {
    drush_log(dt('Imported and/or updated @count products.', array('@count' => $imported_count)), 'ok');
  }
  else {
    drush_log(dt('No products to import.'), 'warning');
  }
}

/**
 * Drush command callback for importing updated or new products into Drupal.
 */
function drush_magento_products_magento_process_queue() {
  module_load_include('inc', 'magento_products');
  $imported_count = magento_products_process_queue();

  if ($imported_count) {
    drush_log(dt('Imported and/or updated @count products.', array('@count' => $imported_count)), 'ok');
  }
  else {
    drush_log(dt('No products to import.'), 'warning');
  }
}
