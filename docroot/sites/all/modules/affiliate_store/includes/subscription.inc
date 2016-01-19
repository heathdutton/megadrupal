<?php

/**
 * @file
 * Affiliate store subscription related operations.
 */

/**
 * Add a subscription to local affiliate store subscription list.
 *
 * @param array $subscription
 *   Subscription associative array with the following keys:
 *   - id: Subscription ID.
 *   - rev: Subscription revision number.
 *   - network: Network name.
 *   - merchant: Merchant name.
 *   - category: (optional) Category name.
 *   - subcategory: (optional) Subcategory name.
 *   - subsubcategory: (optional) Subsubcategory name.
 *
 * @see affiliate_store_subscription_delete()
 */
function affiliate_store_subscription_add($subscription) {
  // Set subscription default values.
  $subscription += array(
    'category' => '',
    'subcategory' => '',
    'subsubcategory' => '',
  );
  $fields = array(
    'ssnid' => $subscription['id'],
    'rev' => $subscription['rev'],
    'hub_rev' => $subscription['rev'],
  );
  foreach (affiliate_store_vocabularies() as $vocab => $info) {
    $tid = affiliate_store_get_term_id($subscription[$vocab], $vocab);
    $fields[$info['as'] . 'tid'] = $tid;
  }
  db_insert('affiliate_store_subscription')
    ->fields($fields)
    ->execute();
}

/**
 * Delete a subscription from local affiliate store subscription list.
 *
 * @param int $id
 *   Subscription ID to delete.
 *
 * @see affiliate_store_subscription_add()
 */
function affiliate_store_subscription_delete($id) {
  db_delete('affiliate_store_subscription')
    ->condition('ssnid', $id)
    ->execute();
}

/**
 * Set subscription refetch status.
 *
 * @param mixed $id
 *   A single subscription ID or an array of subscription IDs.
 * @param boolean $refetch
 *   (optional) Refetch status value. Default to TRUE.
 */
function affiliate_store_subscription_refetch($id, $refetch = TRUE) {
  $id = is_array($id) ? $id : array($id);
  db_update('affiliate_store_subscription')
    ->fields(array('refetch' => $refetch ? 1 : 0))
    ->condition('ssnid', $id)
    ->execute();
}

/**
 * Set subscription local revision.
 *
 * @param mixed $id
 *   A single subscription ID or an array of subscription IDs.
 * @param int $rev
 *   Revision number to set.
 */
function affiliate_store_subscription_set_revision($id, $rev) {
  $id = is_array($id) ? $id : array($id);
  db_update('affiliate_store_subscription')
    ->fields(array('rev' => $rev))
    ->condition('ssnid', $id)
    ->execute();
}

/**
 * Set subscription Hub revision.
 *
 * The Hub revision to be set is the global revision received from Hub when a
 * subscription is updated, which is different from subscription revision.
 *
 * @param mixed $id
 *   A single subscription ID or an array of subscription IDs.
 * @param int $rev
 *   Hub revision number to set.
 */
function affiliate_store_subscription_set_hub_revision($id, $rev) {
  $id = is_array($id) ? $id : array($id);
  db_update('affiliate_store_subscription')
    ->fields(array('hub_rev' => $rev))
    ->condition('ssnid', $id)
    ->execute();
}

/**
 * Synchronize local subscription list with Hub.
 *
 * Keeps track of subscriptions that are added, updated, or deleted and put
 * them in batch queue.
 *
 * @param array &$errors
 *   Return error messages if any.
 */
function affiliate_store_subscription_sync(&$errors) {
  $errors = array();

  affiliate_store_include('service');
  $response = affiliate_store_service_subscription_index();
  if ($response->code != 200) {
    _affiliate_store_subscription_sync_error($response, $errors);
    return;
  }

  $ids = array_keys($response->data['subscriptions']);
  $ids_local = array();
  $ids_refetch = array();
  $ids_update = array();
  $ids_no_update = array();

  // Compare Hub subscription list with local subscription list to detect
  // changes.
  $result = db_query(
    "SELECT ssnid AS id, rev, hub_rev, refetch
    FROM {affiliate_store_subscription}"
  );
  foreach ($result as $ssn) {
    $ids_local[] = $ssn->id;
    if ($ssn->refetch) {
      $ids_refetch[] = $ssn->id;
      continue;
    }

    if (isset($response->data['subscriptions'][$ssn->id])) {
      $rev = $response->data['subscriptions'][$ssn->id]['rev'];
      // Update local subscription revision.
      if ($rev != $ssn->rev) {
        affiliate_store_subscription_set_revision($ssn->id, $rev);
      }
      // Subscription needs update if its revision is greater than last update
      // Hub revision.
      if ($rev <= $ssn->hub_rev) {
        $ids_no_update[] = $ssn->id;
      }
      else {
        $ids_update[] = $ssn;
      }
    }
  }
  // Modify last update Hub revision for up to date subscriptions.
  if ($ids_no_update) {
    affiliate_store_subscription_set_hub_revision(
      $ids_no_update, $response->data['rev']
    );
  }

  affiliate_store_include('batch');
  // Queue subscriptions that have update.
  foreach ($ids_update as $ssn) {
    $batch = array(
      'data' => array('id' => $ssn->id, 'hub_rev' => $ssn->hub_rev),
      'function' => 'affiliate_store_subscription_batch_update',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'subscription:update');
  }
  // Queue added subscriptions.
  foreach (array_diff($ids, $ids_local) as $id) {
    $batch = array(
      'data' => $response->data['subscriptions'][$id],
      'function' => 'affiliate_store_subscription_batch_add',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'subscription:add');
  }
  // Queue deleted subscriptions.
  $deleted = array_diff($ids_local, $ids);
  if ($deleted) {
    $batch = array(
      'data' => $deleted,
      'function' => 'affiliate_store_subscription_batch_delete',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'subscription:delete');
  }
  // Queue subscriptions to be refetched, except those that will be deleted.
  foreach (array_diff($ids_refetch, $deleted) as $id) {
    $batch = array(
      'data' => $id,
      'function' => 'affiliate_store_subscription_batch_refetch',
      'file' => 'subscription.batch',
    );
    affiliate_store_batch_queue($batch, 'subscription:refetch');
  }
}

/**
 * Handle subscription synchronization service call error.
 *
 * @param object $response
 *   Service response object from the call that results in error.
 * @param array &$errors
 *   A reference to an array for returning error messages.
 */
function _affiliate_store_subscription_sync_error($response, &$errors) {
  affiliate_store_service_error($response, $errors);
}
