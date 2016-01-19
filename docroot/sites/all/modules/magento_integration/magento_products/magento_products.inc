<?php

/**
 * @file
 * Helper functions for the Drupal Magento Product integration module.
 */

/**
 * Import updated products into Magento.
 *
 * @return int
 *  The number of Magento products imported.
 */
function magento_products_import() {
  module_load_include('class.inc', 'magento_products');

  $magento_connection = new MagentoConnection();
  $magento = new MagentoProducts($magento_connection);

  // Find the a set of changed products since our last sync to update.
  $magento_date_last_sync = date('Y-m-d H:i:s', variable_get('magento_product_last_sync', 0));
  $products_to_update = $magento->getProductsUpdatedAfter($magento_date_last_sync);
  $product_ids_to_import = array();
  if (!empty($products_to_update)) {
    foreach ($products_to_update as $product) {
      $values = array(
        ':magento_product_id' => $product['product_id'],
        ':created' => time(),
      );
      db_query('INSERT INTO {magento_products_queue} (magento_product_id, created) VALUES (:magento_product_id, :created) ON DUPLICATE KEY UPDATE magento_product_id = magento_product_id', $values);
    }

    // Track our last import time so that next time we run we will grab only newly
    // updated products into our queue.
    variable_set('magento_product_last_sync', time());
  }
  else {
    $message = 'No new products in Magento have updated since @time.';
    $variables = array('@time' => $magento_date_last_sync);
    watchdog('magento_products', $message, $variables);
  }

  // Now that the queue is updated we work down the queue in chunks.
  return magento_products_process_queue();
}

/**
 * Process queued imports in Magento.
 */
function magento_products_process_queue() {
  module_load_include('class.inc', 'magento_products');

  $magento_connection = new MagentoConnection();
  $magento = new MagentoProducts($magento_connection);

  $product_ids_to_import = array();
  $limit = variable_get('magento_products_import_limit', 50);
  $result = db_query_range('SELECT magento_product_id FROM {magento_products_queue} ORDER BY created ASC', 0, $limit);
  foreach ($result as $row) {
    $product_ids_to_import[] = $row->magento_product_id;
  }

  // If the queue is empty, we don't need to do anything else except log and
  // exit.
  if (empty($product_ids_to_import)) {
    $message = 'No products in the import queue';
    watchdog('magento_products', $message, array(), WATCHDOG_NOTICE);
    return;
  }

  // Load up our product batch from Magento and process.
  $products = $magento->getProducts($product_ids_to_import);
  $account = user_load(1);
  $products_updated_count = 0;
  $products_inserted_count = 0;
  foreach ($products as $product) {
    magento_products_import_product($product, $products_inserted_count, $products_updated_count);
  }

  // Even if we failed we want to delete all the products we queued up.
  // Otherwise, the queue may never shrink.
  db_query('DELETE FROM {magento_products_queue} WHERE magento_product_id IN (:magento_product_ids)', array(
    ':magento_product_ids' => $product_ids_to_import,
  ));

  $message = 'Magento Product import complete. Imported @added_count and updated @updated_count.';
  $variables = array(
    '@updated_count' => format_plural($products_updated_count, '@count product', '@count products'),
    '@added_count' => format_plural($products_inserted_count, '@count product', '@count products'),
  );
  watchdog('magento_products', $message, $variables, WATCHDOG_NOTICE);

  return $products_updated_count + $products_inserted_count;
}

/**
 * Import or update an individual Magento Product.
 *
 * @param  array $product
 *   A Magento product array, as returned by the API.
 * @param  integer $products_inserted_count
 *   If your calling method is counting successful updates and inserts, pass
 *   this variable in and it will be updated by reference.
 * @param  integer $products_updated_count
 *   If your calling method is counting successful updates and inserts, pass
 *   this variable in and it will be updated by reference.
 * @return mixed
 *   Node object on successful import or false otherwise.
 */
function magento_products_import_product($product, &$products_inserted_count = 0, &$products_updated_count = 0) {
  try {
    $account = user_load(1);

    // Match to existing node if possible.
    $exists = db_query('SELECT entity_id FROM {magento_products} WHERE magento_product_id = :magento_product_id', array(
      ':magento_product_id' => $product['product_id'],
    ))->fetchField();
    if ($exists) {
      $node = node_load($exists);
    }
    else {
      // Save a new node.
      $node = new stdClass();
      $node->type = 'magento_product';
      node_object_prepare($node);
      $node->language = LANGUAGE_NONE;
      $node->uid = $account->uid;
      $node->name = $account->name;
      $node->comment = 0;
      $node = node_submit($node);
    }

    // In the case of node creation of update, make our changes.
    $node->status = $product['status'] == 1 && (int) $product['visibility'] >= MAGENTO_PRODUCTS_CATALOG_VISIBLITY;
    $node->title = $product['name'];
    $body = $product['description'];
    $node->body[LANGUAGE_NONE][0]['value']   = $body;
    $node->body[LANGUAGE_NONE][0]['summary'] = text_summary($body);
    $node->body[LANGUAGE_NONE][0]['format']  = 'filtered_html';
    $url = variable_get('magento_store_url');
    $node->field_magento_url[LANGUAGE_NONE][0]['value'] = $url . '/' . $product['url_path'];
    $node->field_magento_price[LANGUAGE_NONE][0]['value'] = $product['price'];
    $node->field_magento_brand_id[LANGUAGE_NONE][0]['value'] = $product['brand_name'];
    $node->field_magento_designer_id[LANGUAGE_NONE][0]['value'] = $product['designer_name'];
    $node->field_magento_visibility[LANGUAGE_NONE][0]['value'] = $product['visibility'];
    // Add in the first image URL to the product.
    if (isset($product['primary_image'])) {
      $node->field_magento_image_url[LANGUAGE_NONE][0]['value'] = $product['primary_image']['url'];
    }

    // Let other modules change what is imported.
    drupal_alter('magento_node', $node, $product);

    node_save($node);

    // Create or update the node.
    $bundle = variable_get('magento_product_node_content_type', 'magento_product');
    $values = array(
      ':entity_type' => 'node',
      ':entity_id' => $node->nid,
      ':bundle' => $bundle,
      ':magento_product_id' => $product['product_id'],
      ':synced' => time(),
    );

    // Create a link between Drupal or Magento or update the sync date on our
    // existing link. Also, logging.
    if ($exists) {
      db_query('UPDATE {magento_products} SET synced = :synced WHERE entity_id = :entity_id AND entity_type = :entity_type AND bundle = :bundle AND magento_product_id = :magento_product_id', $values);

      // Log to Drupal.
      $message = 'Updated existing Magento Product in Drupal for %title (nid @nid).';
      $variable = array('%title' => $node->title, '@nid' => $node->nid);
      watchdog('magento_products', $message, $variable, WATCHDOG_NOTICE, url('node/' . $node->nid));

      $products_updated_count++;
    }
    else {
      db_query('INSERT INTO {magento_products} (entity_id, entity_type, bundle, magento_product_id, synced) VALUES (:entity_id, :entity_type, :bundle, :magento_product_id, :synced)', $values);

      // Log to Drupal.
      $message = 'Created new Magento Product in Drupal for %title (nid @nid).';
      $variable = array('%title' => $node->title, '@nid' => $node->nid);
      watchdog('magento_products', $message, $variable, WATCHDOG_NOTICE, url('node/' . $node->nid));

      $products_inserted_count++;
    }

    return $node;
  }
  catch (Exception $e) {
    // Log to Drupal the full error message for debugging.
    $message = 'Unable to import Magento Product (@magento_product_id). Debugging: <pre>!debugging</pre>';
    $variables = array(
      '@magento_product_id' => $product['product_id'],
      '!debugging' => print_r($e, TRUE),
    );
    watchdog('magento_products', $message, $variables, WATCHDOG_ERROR);

    return FALSE;
  }
}
