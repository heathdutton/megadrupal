<?php

/**
 * Devel Generate batch handling functions using the BatchAPI
 */

/**
 * Functions called from FAPI:
 */

function commerce_devel_generate_batch_products($form_state) {
  $operations = array();

  // Setup the batch operations and save the variables.
  $operations[] = array('commerce_devel_generate_batch_products_pre_product', array($form_state['values']));

  // add the kill operation
  if ($form_state['values']['kill_products']) {
    $operations[] = array('commerce_devel_generate_batch_products_kill', array());
  }

  // add the operations to create the products
  for ($num = 0; $num < $form_state['values']['num_products']; $num ++) {
    $operations[] = array('commerce_devel_generate_batch_products_add_product', array());
  }

  // start the batch
  $batch = array(
    'title' => t('Generating Products'),
    'operations' => $operations,
    'finished' => 'commerce_devel_generate_batch_finished',
    'file' => drupal_get_path('module', 'commerce_devel_generate') . '/devel_generate_batch.inc',
  );
  batch_set($batch);
}

function commerce_devel_generate_batch_orders($form_state) {
  $batch = array(
    'title' => t('Generating Orders'),
    'operations' => array(),
    'finished' => 'commerce_devel_generate_batch_orders_finished',
    'file' => drupal_get_path('module', 'commerce_devel_generate') . '/devel_generate_batch.inc',
  );

  if ($form_state['values']['kill_orders']) {
    $batch['operations'][] = array('commerce_devel_generate_batch_orders_kill', array($form_state['values']['kill_transactions']));
  }

  $batch['operations'][] = array('commerce_devel_generate_batch_orders_add', array($form_state['values']));

  batch_set($batch);
}

/**
 * Create Product Batch Functions:
 */

function commerce_devel_generate_batch_products_kill(&$context) {
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  commerce_devel_generate_products_kill($context['results']);
}

function commerce_devel_generate_batch_products_pre_product($vars, &$context) {
  $context['results'] = $vars;
  $context['results']['num_products'] = 0;
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  commerce_devel_generate_products_pre_product($context['results']);
}

function commerce_devel_generate_batch_products_add_product(&$context) {
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  module_load_include('inc', 'devel_generate', 'devel_generate');
  commerce_devel_generate_product_add_product($context['results']);
  $context['results']['num_product_ids'] ++;
}

function commerce_devel_generate_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t('Finished @num_product_ids products created successfully.', array('@num_product_ids' => $results['num_product_ids']));
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

/**
 * Create Order Batch Functions:
 */

/**
 * Deletes all existing orders.
 *
 * @param $kill_transactions
 *    Boolean indicating that the order's transactions should be removed as well.
 */
function commerce_devel_generate_batch_orders_kill($kill_transactions, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['orders'] = db_query('SELECT order_id FROM {commerce_order}')->fetchCol();
    $context['sandbox']['max'] = count($context['sandbox']['orders']);
    $context['sandbox']['progress'] = 0;
  }

  if (!empty($context['sandbox']['orders'])) {
    $order_ids = array_slice($context['sandbox']['orders'], 0, 100);
    $context['sandbox']['orders'] = array_slice($context['sandbox']['orders'], 100);

    commerce_order_delete_multiple($order_ids);

    if ($kill_transactions) {
      $transaction_ids = db_query('SELECT transaction_id FROM {commerce_payment_transaction} WHERE order_id IN (:ids)', array(':ids' => $order_ids))->fetchCol();

      commerce_payment_transaction_delete_multiple($transaction_ids);
    }

    $context['sandbox']['progress'] += count($order_ids);
  }

  if (!empty($context['sandbox']['orders'])) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    drupal_set_message(t('Deleted %count orders.', array('%count' => $context['sandbox']['max'])));
  }
}

/**
 * Generate new orders.
 *
 * $options
 *    Mixed array containing options retrieved through the Form API.
 */
function commerce_devel_generate_batch_orders_add($options, &$context) {
  if (empty($context['sandbox'])) {
    module_load_include('inc', 'devel_generate');

    // Options.
    $context['sandbox']['options'] = $options;
    $context['sandbox']['options']['start'] = mktime(0, 0, 0, $options['start']['month'], $options['start']['day'], $options['start']['year']);
    $context['sandbox']['options']['end'] = mktime(23, 59, 59, $options['end']['month'], $options['end']['day'], $options['end']['year']);

    // Entities used during generation.
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'commerce_product')->execute();

    $context['sandbox']['products'] = $results['commerce_product'];
    $context['sandbox']['users'] = array_merge(devel_get_users(), array('0'));

    // Progress of the generation.
    $context['sandbox']['max'] = $context['sandbox']['options']['amount'];
    $context['sandbox']['progress'] = 0;

    // Results.
    $context['results']['orders'] = array();
    $context['results']['transactions'] = array();
  }

  $batch_size = min(50, $context['sandbox']['max'] - $context['sandbox']['progress']);

  for ($x = 0; $x < $batch_size; $x ++) {
    $uid = $context['sandbox']['users'][array_rand($context['sandbox']['users'])];
    $date = mt_rand($context['sandbox']['options']['start'], $context['sandbox']['options']['end']);
    $product_count = mt_rand($context['sandbox']['options']['min_products'], $context['sandbox']['options']['max_products']);

    $order = commerce_cart_order_new($uid);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    for ($i = 0; $i < $product_count; $i ++) {
      $product_entity = $context['sandbox']['products'][array_rand($context['sandbox']['products'])];

      $product = commerce_product_load($product_entity->product_id);

      $line_item = commerce_product_line_item_new($product, 1);
      $line_item->order_id = $order->order_id;
      $line_item->created = $date;
      commerce_line_item_save($line_item);

      $order_wrapper->commerce_line_items[] = $line_item;
    }

    $order->created = $date;

    $status = array_rand($context['sandbox']['options']['order_statuses']);

    if ($status != $order->status) {
      $order = commerce_order_status_update($order, $status, TRUE);
    }

    commerce_order_save($order);

    $context['results']['orders'][] = $order;

    if ($context['sandbox']['options']['create_transactions']) {
      $method = array_rand($context['sandbox']['options']['payment_method']);

      $transaction = commerce_payment_transaction_new($method, $order->order_id);
      $transaction->uid = $uid;
      $transaction->amount = $order->commerce_order_total['und'][0]['amount'];
      $transaction->created = $order->created;
      $transaction->status = 'success';

      commerce_payment_transaction_save($transaction);

      $context['results']['transactions'][] = $transaction;
    }

    $context['sandbox']['progress'] ++;
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finished callback in Batch API for order generation.
 */
function commerce_devel_generate_batch_orders_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(format_plural(count($results['orders']), 'Created 1 order', 'Created @count orders'));

    if (count($results['transactions'])) {
      drupal_set_message(format_plural(count($results['transactions']), 'Created 1 transaction', 'Created @count transactions'));
    }
  }
  else {
    drupal_set_message(t('Finished with an error.'), 'error');
  }
}
