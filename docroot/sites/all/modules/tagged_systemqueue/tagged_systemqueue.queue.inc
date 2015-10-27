<?php

/**
 * @file
 * Implements the TaggedSystemQueue class.
 *
 * @todo: Find a way to tag claimItem method's 'db_update' op (looks
 *        like db_update() object has no addTag() method).
 * @todo: Find other taggable methods and queries.
 */

class TaggedSystemQueue extends SystemQueue implements DrupalReliableQueueInterface {

  public function claimItem($lease_time = 30) {
    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    while (TRUE) {
      $query = db_select('queue', 'q')
        ->fields('q')
        ->condition('expire', 0, '=')
        ->condition('name', $this->name, '=')
        ->orderBy('created')
        ->orderBy('item_id')
        ->range(0, 1);
      $query = $this->_addQueryTags($query, __FUNCTION__);
      $query = $this->_addQueryMetaData($query, __FUNCTION__, 'db_query_range');

      $item = $query->addMetaData('lease_time', $lease_time)
        ->execute()
        ->fetchObject();
      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $update = db_update('queue')
          ->fields(array(
            'expire' => time() + $lease_time,
          ))
          ->condition('item_id', $item->item_id)
          ->condition('expire', 0);
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

  private function _addQueryTags(QueryAlterableInterface $query, $method) {
    return $query
      ->addTag('queue')
      ->addTag($this->name)
      ->addTag('queue_' . $method);
  }

  private function _addQueryMetaData(QueryAlterableInterface $query, $method, $op) {
    return $query
      ->addMetaData('name', $this->name)
      ->addMetaData('method', $method)
      ->addMetaData('op', $op);
  }

}
