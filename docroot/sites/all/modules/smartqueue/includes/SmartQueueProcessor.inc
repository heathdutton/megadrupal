<?php

// The maximum amount of time to process during a single attempt.
define('SMARTQUEUE_MAX_PROCESS_TIME', 45);

// Duration of a lock acquired my a SmartQueue process.
define('SMARTQUEUE_LOCK_DURATION', 50);

// PHP execution limit imposed on a SmartQueue process.
define('SMARTQUEUE_PHP_TIMEOUT', 55);

interface SmartQueueProcessor {

  /**
   * Process the queue.
   *
   * @return Boolean
   *   Value indicating whether or not the queue is complete, meaning no further
   *   items to process.
   */
  public function process();

}

class SmartQueueProcessorDefault implements SmartQueueProcessor {

  /**
   * @var integer
   *
   * If unable to complete all queued items, spend this long attempting to
   * process the queue.
   */
  public $maxProcessTime = SMARTQUEUE_MAX_PROCESS_TIME;

  /**
   * @var integer
   *
   * Amount of time, measured in seconds, to acquire locks for queue items. This
   * needs to be long enough to ensure that the processing attempt has been
   * stopped before another process attempts to process the queue item.
   */
  public $lockDuration = SMARTQUEUE_LOCK_DURATION;

  /**
   * @var integer
   *
   * The PHP timeout will be set to this value before ending the process. This
   * should be large enough to allow queue item processing to end gracefully
   * before hitting this hard, ungraceful limit.
   */
  public $phpTimeout = SMARTQUEUE_PHP_TIMEOUT;

  /**
   * @inherit
   */
  public function process() {
    drupal_set_time_limit($this->phpTimeout);

    timer_start(__FUNCTION__);
    $count = 0;

    do {
      // Get a queue item to process.
      if (!$item = $this->next()) {
        // Nothing left to process, we're done! Wahoo!
        return TRUE;
      }

      $item->process();
      $count++;

      // Elapsed time.
      $et = timer_read(__FUNCTION__) / 1000;
      // Estimated time to process 1 item.
      $est = $et / $count;

      $continue = ($this->maxProcessTime) ?  $et + $est < $this->maxProcessTime : TRUE;
    } while ($continue);

    return FALSE;
  }

  /**
   * @inherit
   */
  protected function next() {
    return smartqueue_load($this->nextItem());
  }

  /**
   * Gets the next workable item from the queue.
   *
   * @param int $min
   *   The qid of the last checked queue item.
   * @return int
   *   The qid of the next queue item ready for processing.
   */
  protected function nextItem($min = NULL) {
    if (!$qid = $this->query($min)) {
      // No results.
      return;
    }

    // Attempt to acquire lock for right to process.
    $lock_name = 'smartqueue_' . $qid;

    // If unable, try again.
    if (!lock_acquire($lock_name, $this->lockDuration)) {
      $qid = $this->nextItem($qid);
    }

    return $qid;
  }

  /**
   * Query the next queue item from the database.
   *
   * @param int $min
   *   The qid of the last checked queue item.
   * @return int
   *   The qid of the next queue item ready for processing.
   */
  protected function query($min = NULL) {
    $args = array();

    if ($min) {
      $query = 'SELECT qid FROM {smartqueue} WHERE qid > ? AND status = 0 LIMIT 1';
      $args[] = $min;
    } else {
      $query = 'SELECT qid FROM {smartqueue} WHERE status = 0 LIMIT 1';
    }

    return db_query($query, $args)->fetchField();
  }

}
