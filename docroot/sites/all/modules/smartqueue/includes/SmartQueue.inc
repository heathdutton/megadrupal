<?php

interface SmartQueue {

  /**
   * Initialize a SmartQueue item.
   */
  public function __construct($data = array());

  /**
   * Worker function for processing the queue item.
   */
  public function process();

  /**
   * Save the queue.
   */
  public function save();

  /**
   * Sets queue data as loaded from the database.
   *
   * @param stdClass $record
   *   A record loaded from the database.
   */
  public function setRecord($record);

  /**
   * Mark the queue process complete.
   *
   * @param int $status
   *   The status to set the queue item. Defaults to complete, may be used to
   *   set the status to other values.
   */
  public function markComplete($status = 1);

}

/**
 * Base class implementing low-level SmartQueue functionality.
 */
abstract class SmartQueueBase implements SmartQueue {

  public $qid = NULL;
  public $type = 'smartqueue';
  public $status = 0;
  public $uid;
  public $created;
  public $updated;
  public $data;

  /**
   * @inherit
   */
  public function __construct($data = array()) {
    global $user;
    $this->uid = $user->uid;
    $this->class = get_class($this);
    $this->data = $data;
  }

  /**
   * @inherit
   */
  public function setRecord($record) {
    $this->qid = $record->qid;
    $this->status = $record->status;
    $this->uid = $record->uid;
    $this->created = $record->created;
    $this->updated = $record->updated;
    $this->data = unserialize($record->data);
  }

  /**
   * @inherit
   */
  public function save() {

    if (!isset($this->created)) {
      $this->created = time();
    }
    $this->updated = time();

    if (isset($this->qid)) {
      // Update.
      drupal_write_record('smartqueue', $this, 'qid');
    } else {
      // Insert.
      drupal_write_record('smartqueue', $this);
    }

  }

  /**
   * @inherit
   */
  public function markComplete($status = 1) {
    $this->status = $status;
    $this->save();
  }

}
