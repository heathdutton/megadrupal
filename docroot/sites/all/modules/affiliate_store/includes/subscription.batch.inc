<?php

/**
 * @file
 * Affiliate store subscription batch processing functions.
 */

/**
 * Batch queue handler; Add new subscription.
 *
 * @param array &$data
 *   Subscription associative array with the following keys:
 *   - id: Subscription ID.
 *   - rev: Subscription revision number.
 *   - network: Network name.
 *   - merchant: Merchant name.
 *   - category: (optional) Category name.
 *   - subcategory: (optional) Subcategory name.
 *   - subsubcategory: (optional) Subsubcategory name.
 *
 * @see affiliate_store_subscription_batch_update()
 */
function affiliate_store_subscription_batch_add(&$data, &$context) {
  affiliate_store_subscription_batch_update($data, $context);

  $context['message'] = t(
    'Fetching new subscription: Part @part.',
    array('@part' => $context['sandbox']['progress'])
  );

  if (_affiliate_store_subscription_batch_finished($context)) {
    // Add the new subscription to local subscription list.
    affiliate_store_include('subscription');
    affiliate_store_subscription_add($data);
  }
}

/**
 * Batch queue handler; Refetch subscription.
 *
 * @param int &$data
 *   Subscription ID to refetch.
 *
 * @see affiliate_store_subscription_batch_update()
 */
function affiliate_store_subscription_batch_refetch(&$data, &$context) {
  if (!isset($context['sandbox'][__FUNCTION__]['data'])) {
    // Store original data.
    $context['sandbox'][__FUNCTION__]['data'] = $data;
    $data = array('id' => $data);
  }

  affiliate_store_subscription_batch_update($data, $context);

  $context['message'] = t(
    'Refetching existing subscription: Part @part.',
    array('@part' => $context['sandbox']['progress'])
  );

  if (_affiliate_store_subscription_batch_finished($context)) {
    // Delete products that do not exist in subscription anymore. Since a
    // refetch updates all active products, non-exist products will have stale
    // last update timestamp.
    affiliate_store_include('batch');
    affiliate_store_include('subscription');
    $data = $context['sandbox'][__FUNCTION__]['data'];
    $batch = array(
      'data' => array('id' => $data, 'before_time' => REQUEST_TIME),
      'function' => 'affiliate_store_subscription_product_batch_delete',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'product:delete');
    affiliate_store_subscription_refetch($data, FALSE);
  }
}

/**
 * Batch queue handler; Delete subscriptions and their products.
 *
 * @param array &$data
 *   Subscription IDs to delete.
 */
function affiliate_store_subscription_batch_delete(&$data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($data);
    if (!$context['sandbox']['total']) {
      return;
    }
  }

  $context['message'] = t(
    'Deleting subscriptions: @current of @total.',
    array(
      '@current' => $context['sandbox']['progress'] + 1,
      '@total' => $context['sandbox']['total'],
    )
  );

  $id = current($data);
  $on = array();
  foreach (affiliate_store_vocabularies() as $info) {
    $on[] = "s.{$info['as']}tid = p.{$info['as']}tid";
  }
  $products = db_query(
    "SELECT n.nid, n.title FROM {node} AS n
    JOIN {affiliate_store_product} AS p ON p.vid = n.vid
    JOIN {affiliate_store_subscription} AS s ON " . implode(' AND ', $on) . "
    WHERE s.ssnid = :ssnid",
    array(':ssnid' => $id)
  )->fetchAllAssoc('nid');

  if ($products) {
    affiliate_store_include('batch');
    $batch = array(
      'data' => array('products' => $products),
      'function' => 'affiliate_store_subscription_product_batch_delete',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'product:delete');
  }
  affiliate_store_include('subscription');
  affiliate_store_subscription_delete($id);
  array_shift($data);

  ++$context['sandbox']['progress'];
  $context['finished'] = (
    $data ? ($context['sandbox']['progress'] / $context['sandbox']['total']) : 1
  );
}

/**
 * Batch queue handler; Update a subscription.
 *
 * @param array &$data
 *   Subscription associative array with the following keys:
 *   - id: string Subscription ID.
 *   - hub_rev: int (optional) Hub revision number when subscription last
 *     updated. Default to 0.
 */
function affiliate_store_subscription_batch_update(&$data, &$context) {
  affiliate_store_include('service');
  if (!isset($context['sandbox']['data'])) {
    // Fetch update data from Hub for the first time.
    $data += array('hub_rev' => 0);
    $context['sandbox'] += array('progress' => 0, 'error' => 0);
    $response = affiliate_store_service_subscription_retrieve(
      $data['id'],
      array(
        'rev' => $data['hub_rev'],
        'limit' => variable_get('affiliate_store_retrieve_limit', 0),
      )
    );
  }
  else {
    // Resume previous update.
    $url = &$context['sandbox']['data']['links']['next']['url'];
    if (!$url) {
      // Shouldn't be here as empty next link would already delete the batch.
      return;
    }
    $response = affiliate_store_service_subscription_retrieve($url);
  }

  if ($response->code != 200) {
    // Make batch data available for error handler.
    $context['sandbox']['batch'] = $data;
    $context['results']['errors'] = array();
    _affiliate_store_subscription_batch_update_error(
      $response, $context, $context['results']['errors']
    );
    return;
  }

  $context['sandbox']['data'] = $response->data;
  ++$context['sandbox']['progress'];
  $context['message'] = t(
    'Updating existing subscription: Part @part.',
    array('@part' => $context['sandbox']['progress'])
  );

  // Queue product update. Products key can be empty if there is no update.
  if (!empty($response->data['subscription']['products'])) {
    affiliate_store_include('batch');
    $batch = array(
      'data' => $response->data['subscription'],
      'function' => 'affiliate_store_subscription_product_batch_update',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'product:update');
  }

  if (!empty($response->data['links']['next'])) {
    // Estimation, just to let progress bar keep going since total count can't
    // be obtained in multipart response.
    $context['finished'] = min($context['sandbox']['progress'] / 10, 0.99);
  }
  else {
    // Finished, update subscription last update Hub revision number.
    affiliate_store_include('subscription');
    affiliate_store_subscription_set_hub_revision(
      $response->data['subscription']['id'], $response->data['rev']
    );
  }
}

/**
 * Batch queue handler; Delete subscription products.
 *
 * @param array &$data
 *   An associative array with the following keys:
 *   - products: array (optional) Products to delete, each product is an object
 *     with the following properties:
 *     - nid: int Product node ID.
 *     - title: string Product title.
 *     Only when this key is empty, the keys below will be used to get the
 *     products to delete.
 *   - id: int (optional) Subscription ID.
 *   - before_time: int (optional) Only delete products that have not been
 *     updated since this timestamp. Default to max meaning delete all products.
 */
function affiliate_store_subscription_product_batch_delete(&$data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $data += array('products' => array(), 'before_time' => PHP_INT_MAX);
    // Get products to delete if not supplied.
    if (empty($data['products']) && isset($data['id'])) {
      $on = array();
      foreach (affiliate_store_vocabularies() as $info) {
        $on[] = "s.{$info['as']}tid = p.{$info['as']}tid";
      }
      $data['products'] += db_query(
        "SELECT n.nid, n.title FROM {node} AS n
        JOIN {affiliate_store_product} AS p ON p.vid = n.vid
        JOIN {affiliate_store_subscription} AS s
             ON " . implode(' AND ', $on) . "
        WHERE s.ssnid = :ssnid AND n.changed < :changed",
        array(':ssnid' => $data['id'], ':changed' => $data['before_time'])
      )->fetchAllAssoc('nid');
    }

    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($data['products']);
    if (!$context['sandbox']['total']) {
      return;
    }
  }

  $context['message'] = t(
    'Deleting old subscription products: @current of @total.',
    array(
      '@current' => $context['sandbox']['progress'] + 1,
      '@total' => $context['sandbox']['total'],
    )
  );

  $product = current($data['products']);
  node_delete($product->nid);
  array_shift($data['products']);

  $context['results']['product:deleted'][] = t(
    'Deleted product %title', array('%title' => $product->title)
  );
  ++$context['sandbox']['progress'];
  $context['finished'] = (
    $data['products'] ?
    ($context['sandbox']['progress'] / $context['sandbox']['total']) : 1
  );

  if ($context['finished'] == 1) {
    // Clear affiliate store caches so that old categories don't show up.
    affiliate_store_include('common');
    affiliate_store_clear_cache();
  }
}

/**
 * Batch queue handler; Update subscription products.
 *
 * @param array &$data
 *   Subscription associative array with products to update.
 */
function affiliate_store_subscription_product_batch_update(&$data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($data['products']);
    if (!$context['sandbox']['total']) {
      return;
    }
  }

  $context['message'] = t(
    'Updating subscription products: @current of @total.',
    array(
      '@current' => $context['sandbox']['progress'] + 1,
      '@total' => $context['sandbox']['total'],
    )
  );

  affiliate_store_include('common');
  $product = current($data['products']);
  $product['pid'] = $product['id'];
  foreach (array_keys(affiliate_store_vocabularies()) as $vocab) {
    $product[$vocab] = !empty($data[$vocab]) ? $data[$vocab] : '';
  }
  switch (affiliate_store_save_product($product)) {
    case SAVED_NEW:
      $context['results']['product:added'][] = t(
        "Added product %title", array('%title' => $product->title)
      );
      break;
    case SAVED_UPDATED:
      $context['results']['product:updated'][] = t(
        "Updated product %title", array('%title' => $product->title)
      );
      break;
    case SAVED_DELETED:
      $context['results']['product:deleted'][] = t(
        "Deleted product %title", array('%title' => $product->title)
      );
      break;
  }
  array_shift($data['products']);

  ++$context['sandbox']['progress'];
  $context['finished'] = (
    $data['products'] ?
    ($context['sandbox']['progress'] / $context['sandbox']['total']) : 1
  );

  if ($context['finished'] == 1) {
    // Clear affiliate store caches so that new categories show up.
    affiliate_store_clear_cache();
  }
}

/**
 * Check whether a subscription batch has finish all its operations
 * successfully.
 *
 * @param array $context
 *   Batch context.
 *
 * @return boolean
 *   TRUE if the batch has been completed successfully, FALSE otherwise.
 */
function _affiliate_store_subscription_batch_finished($context) {
  return (($context['finished'] == 1)
    && !$context['requeue']
    && ($context['sandbox']['error'] < AFFILIATE_STORE_BATCH_MAX_ERROR)
  );
}

/**
 * Handle subscription batch update service call error.
 *
 * @param object $response
 *   Service response object from the call that results in error.
 * @param array &$context
 *   A reference to batch context.
 * @param array &$errors
 *   A reference to an array for returning error messages.
 */
function _affiliate_store_subscription_batch_update_error($response,
&$context, &$errors) {
  if ($response->code == 304) {
    // Not real error. Can safely end update as subscription is not modified.
    return;
  }
  elseif (($response->code == 400) && !empty($response->data['errors'])) {
    foreach ($response->data['errors'] as $error) {
      // Refetch subscription that cannot be synchronized incrementally.
      if (($error['code'] == EARNHIGHWAY_REV_OLD)
        || ($error['code'] == EARNHIGHWAY_IDX_REV_OLD)
      ) {
        affiliate_store_include('subscription');
        affiliate_store_subscription_refetch($error['params']['id']);
        return;
      }
    }
  }
  elseif ($response->code == 404) {
    // Subscription has been unsubscribed, delete from local subscription list.
    affiliate_store_include('batch');
    $batch = array(
      'data' => array($context['sandbox']['batch']['id']),
      'function' => 'affiliate_store_subscription_batch_delete',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'subscription:delete');
    return;
  }
  elseif ($response->code == 503) {
    if (!empty($response->headers['retry-after'])) {
      // Requeue the batch to start after the specified time.
      $now = REQUEST_TIME;
      if (is_numeric($response->headers['retry-after'])) {
        $time = $now + $response->headers['retry-after'];
      }
      else {
        $time = (int) strtotime($response->headers['retry-after']);
      }
      // Limit to maximum delay of 1 day.
      $context['requeue'] = min(max($time, $now), $now + 86400);
    }
    else {
      // Assume temporary when there is no Retry-After header, retry later.
      $context['finished'] = min($context['sandbox']['progress'] / 10, 0.99);
      sleep(1);
    }
    return;
  }
  elseif ($response->code == AFFILIATE_STORE_MD5_MISMATCH) {
    // Assume temporary, retry later.
    $context['finished'] = min($context['sandbox']['progress'] / 10, 0.99);
    sleep(1);
    return;
  }

  // If this batch stuck for too long, don't requeue and give up.
  if (++$context['sandbox']['error'] < AFFILIATE_STORE_BATCH_MAX_ERROR) {
    $context['requeue'] = TRUE;
  }
  affiliate_store_service_error($response, $errors);
}
