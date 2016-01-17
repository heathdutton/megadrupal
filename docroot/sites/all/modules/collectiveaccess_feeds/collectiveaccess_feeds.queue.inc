<?php

/**
 * @file
 * Add Queue mechanism for CollectiveAccess Feeds
 */

/**
 * Queue an array of items
 */
function collectiveaccess_feeds_queue_items($feeds_importer_id, $item_ids) {
  if ($item_ids) {
    foreach ($item_ids as $item_id) {
      collectiveaccess_feeds_queue_add($feeds_importer_id, $item_id);
    }
  }
}

/**
 * Queue an item
 */
function collectiveaccess_feeds_queue_add($feeds_importer_id, $item_id) {
  $existing = collectiveaccess_feeds_check_item($feeds_importer_id, $item_id);
  if ($existing) {
    // This item is already queued, no need to add it again.
    return FALSE;
  }
  $entry = array(
    'feeds_importer_id' => $feeds_importer_id,
    'ca_item_id' => $item_id,
  );
  drupal_write_record('collectiveaccess_feeds_queue', $entry);
}

/**
 * Checks if an item already exists in the queue
 */
function collectiveaccess_feeds_check_item($feeds_importer_id, $item_id) {
  $val = db_query("SELECT ca_item_id FROM {collectiveaccess_feeds_queue} WHERE feeds_importer_id = :feeds_importer_id AND ca_item_id = :ca_item_id", array(':feeds_importer_id' => $feeds_importer_id, 'ca_item_id' => $item_id))->fetchField();
  if ($val) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Get a number of queued items for processing
 * @param string $feeds_importer_id: unique ID of feeds importer to get queued items for
 * @param int $number: number of items to fetch
 *
 * @return array containing CollectiveAccess item IDs. Format queue id => CollectiveAccess item id
 */
function collectiveaccess_feeds_get_queued_items($feeds_importer_id, $number = 10) {
  $items = array();
  return db_select('collectiveaccess_feeds_queue', 'cfq')
    ->condition('cfq.feeds_importer_id', $feeds_importer_id, '=')
    ->fields('cfq', array('id', 'ca_item_id'))
    ->range(0, $number)
    ->orderBy('id')
    ->execute()
    ->fetchAllKeyed();
}

/**
 * Remove items from a particular queue
 */
function collectiveaccess_feeds_delete_queue_items($feeds_importer_id, $queue_ids) {
  if (!empty($queue_ids)) {
    return db_delete('collectiveaccess_feeds_queue')
    ->condition('feeds_importer_id', $feeds_importer_id)
    ->condition('id', $queue_ids, 'IN')
    ->execute();
  }
}