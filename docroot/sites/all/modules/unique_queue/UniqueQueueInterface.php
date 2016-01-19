<?php
/**
 * @file
 * Contains UniqueQueueInterface
 */

/**
 * Interface for unique queue items. May be extended to add support for unique
 * queue items to different queue backends.
 */
interface UniqueQueueInterface extends DrupalQueueInterface {
  /**
   * Attempts to create a unique queue item.
   *
   * Uniqueness is only maintained per queue and for non-claimed items. If a
   * queue item with the same name and key exists but has already been claimed
   * a new queue item will be created. This prevents race conditions where the
   * new queue item may need to access data that was not available to the
   * currently processing item.
   *
   * @param string $key
   *   Unique key for the queue item.
   * @param array $data
   *   Arbitrary data to be associated with the new task in the queue.
   * @return boolean
   *   TRUE if the item was successfully created and was (best effort) added
   *   to the queue, otherwise FALSE. We don't guarantee the item was
   *   committed to disk etc, but as far as we know, the item is now in the
   *   queue.
   */
  public function createUniqueItem($key, $data);
}
