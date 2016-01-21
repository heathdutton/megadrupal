<?php

/**
 * Class DropSharkDbQueue.
 */
class DropSharkDbQueue implements DropSharkQueue {

  /** @var DropSharkRequest */
  protected $request;

  /**
   * {@inheritdoc}
   */
  public function __construct($request) {
    $this->request = $request;
  }

  /**
   * {@inheritdoc}
   */
  public function add($data) {
    $record = new stdClass();
    $record->created = $this->timestamp();
    $record->data = json_encode($data);
    drupal_write_record('dropshark_queue', $record);
  }

  /**
   * {@inheritdoc}
   */
  public function process() {
    // Clear any old stuff that didn't finish processing.
    $this->clearLocks();

    // Lock the next X items.
    $key = $this->lock();

    // Attempt to submit items.
    $result = $this->submitItems($key);

    // On successful submission, clear submitted items.
    if (empty($result->error)) {
      $this->removeItems($key);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function clearLocks($timestamp = NULL) {
    if (!$timestamp) {
      $timestamp = $this->timestamp() - variable_get('dropshark_queue_lock_duration_max', DROPSHARK_QUEUE_LOCK_MAX_DEFAULT);
    }
    $query = 'UPDATE {dropshark_queue} SET lock_id = NULL , lock_time = NULL WHERE lock_time < ?';
    db_query($query, array($timestamp));
  }

  /**
   * Provides a timestamp for queue entries.
   *
   * @return int
   *   Unix type timestamp of when the queue item was added.
   */
  protected function timestamp() {
    return time();
  }

  /**
   * Lock a set of items to prevent duplicate processing.
   *
   * @return string
   *   The key of the locked items.
   */
  protected function lock() {
    mt_srand(time() / __LINE__);
    $key = md5(__METHOD__ . microtime() . mt_rand(0, 999999));

    $limit = variable_get('dropshark_queue_max_process', DROPSHARK_QUEUE_MAX_PROCESS_DEFAULT);
    $query = 'UPDATE {dropshark_queue} SET lock_id = ? , lock_time = ? WHERE lock_id IS NULL ORDER BY CREATED LIMIT ' . $limit;
    db_query($query, array($key, $this->timestamp()));

    return $key;
  }

  /**
   * Removes items from the queue by lock.
   *
   * @param string $key
   *   The key of the items to be removed.
   */
  protected function removeItems($key) {
    $query = 'DELETE FROM {dropshark_queue} WHERE lock_id = ?';
    db_query($query, array($key));
  }

  /**
   * Load queued items.
   *
   * @param string $key
   *   The key of the items load.
   *
   * @return array
   */
  protected function getItems($key) {
    $query = 'SELECT data FROM {dropshark_queue} WHERE lock_id = ? ORDER BY created';

    $data = array();
    foreach (db_query($query, array($key)) as $item) {
      $data[] = json_decode($item->data);
    }

    return $data;
  }

  /**
   * Process queued items.
   *
   * @param string $key
   *   The key of the items to be processed.
   *
   * @return stdClass
   *   Result of drupal_http_request() from the data submission.
   */
  protected function submitItems($key) {
    $items = $this->getItems($key);
    $data = json_encode($items);
    return $this->request->post('data', array('data' => $data));
  }

}
