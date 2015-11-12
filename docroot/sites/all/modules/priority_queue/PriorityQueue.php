<?php

/**
 * @file
 * Contains PriorityQueue.
 */

/**
 * Priority queue implementation.
 */
class PriorityQueue extends SystemQueue {

  /**
   * Add a queue item and store it directly to the queue.
   *
   * @param mixed $data
   *   Arbitrary data to be associated with the new task in the queue.
   * @param int $priority
   *   The associated priority.
   *
   * @return bool
   *   TRUE if the item was successfully created and was (best effort) added to
   *   the queue, otherwise FALSE. We don't guarantee the item was committed to
   *   disk etc, but as far as we know, the item is now in the queue.
   */
  public function createItem($data, $priority = 0) {
    // During a Drupal 6.x to 7.x update, drupal_get_schema() does not contain
    // the queue table yet, so we cannot rely on drupal_write_record().
    $query = db_insert('queue')->fields(array(
      'name' => $this->name,
      'data' => serialize($data),

      // We cannot rely on REQUEST_TIME because many items might be created
      // by a single request which takes longer than 1 second.
      'created' => time(),
      'priority' => $priority,
    ));
    return (bool) $query->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function claimItem($lease_time = 30) {
    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    while (TRUE) {
      $item = db_query_range('SELECT data, item_id FROM {queue} q WHERE expire = 0 AND name = :name ORDER BY priority DESC, created ASC, item_id ASC', 0, 1, array(':name' => $this->name))->fetchObject();
      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $update = db_update('queue')->fields(array(
          'expire' => time() + $lease_time,
        ))->condition('item_id', $item->item_id)->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          $item->data = unserialize($item->data);
          return $item;
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }
  }
}
