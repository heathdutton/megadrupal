<?php

/**
 * @file
 * voIP call queue functionality..
 */

/**
 * Default extension of SystemQueue.
 */
class VoipQueue extends AdvancedQueue {
  /**
   * Set default concurrency limit.
   *
   * @param int $limit
   *   The limit. Use -1 for no limit.
   */
  public static function setDefaultLimit($limit) {
    variable_set('voipqueue_limit_default', $limit);
  }

  /**
   * Get default concurrency limit.
   *
   * @return int
   *   Default limit.
   */
  public static function getDefaultLimit() {
    return variable_get('voipqueue_limit_default', -1);
  }

  /**
   * Set lists concurrency limit.
   *
   * @param int $limit
   *   The limit. Use -1 for no limit.
   */
  public function setLimit($limit) {
    variable_set('voipqueue_limit_' . $this->name, $limit);
  }

  /**
   * Get list's concurrency limit.
   */
  public function getLimit() {
    return variable_get('voipqueue_limit_' . $this->name, VoipQueue::getDefaultLimit());
  }

  /**
   * Add item to queue with optional delay.
   *
   * @param mixed|VoipQueueJob $data
   *   Arbitary data to be associated with the new task in the queue.
   *
   * @return bool|int
   *   ID if the item was successfully created and was (best effort) added
   *   to the queue, otherwise FALSE. We don't guarantee the item was
   *   committed to disk etc, but as far as we know, the item is now in the
   *   queue.
   */
  public function createItem($data) {
    if (is_a($data, 'VoipQueueJob')) {
      $query = db_insert('advancedqueue')
        ->fields($data->prepare($this->name));
      $result = $query->execute();

      if ($result) {
        //Insert into voipqueue_calls
        $cid = $data->cid;
        db_insert('voipqueue_calls')
          ->fields(array(
            'item_id' => $result,
            'cid' => $cid,
          ))->execute();

        if(isset($data->tags)) {
          foreach ((array) $data->tags as $tag) {
            $query = db_insert('advancedqueue_tags')
              ->fields(array(
                'item_id' => $result,
                'tag' => $tag,
              ));
            $query->execute();
          }
        }
      }
    }
    else {
      $result = parent::createItem($data);
    }

    return $result;
  }

  public function claimItem($lease_time = 600) {
    // Check concurrency limit.
    if ($this->getLimit() >= 0) {
      $count = $this->numberOfClaimedItems();
      if ($count >= $this->getLimit()) {
        return FALSE;
      }
    }

    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    while (TRUE) {
      $item = db_query_range('SELECT data, item_id FROM {advancedqueue} q WHERE status = ' . ADVANCEDQUEUE_STATUS_QUEUED . ' AND expire = 0 AND name = :name ORDER BY created ASC', 0, 1, array(':name' => $this->name))->fetchObject();
      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $item->data = unserialize($item->data);
        if (is_a($item->data, 'VoipQueueJob')) {
          $lease_time = $item->data->leaseTime;
        }
        $update = db_update('advancedqueue')
          ->fields(array(
            'status' => ADVANCEDQUEUE_STATUS_PROCESSING,
            'expire' => time() + $lease_time,
          ))
          ->condition('item_id', $item->item_id)
          ->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          break;
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }

    if (is_a($item->data, 'VoipQueueJob')) {
      $item->data->item_id = $item->item_id;
      $item = $item->data;
    }

    return $item;
  }

  /**
   * Release an item that the worker could not process, so another
   * worker can come in and process it.
   *
   * If the item should be delayed again reset the $item->enqueued time
   * before releasing it.
   *
   * @param mixed|VoipQueueJob $item
   *   The item to release.
   *
   * @return bool
   *   Result of database update.
   */
  public function releaseItem($item) {
    if (is_a($item, 'VoipQueueJob')) {
      $update = db_update('advancedqueue')
        ->fields($item->prepare($item->queueName))
        ->condition('item_id', $item->item_id);
      $result = $update->execute();
    }
    else {
      $result = parent::releaseItem($item);
    }
    return $result;
  }

  /**
   * Retrieve the number of items claimed.
   *
   * @return int
   *   An integer estimate of the number of items in the queues.
   */
  public function numberOfClaimedItems() {
    //return db_query('SELECT count(item_id) FROM {advancedqueue} WHERE name = :name AND expire > 0 AND status = ' . ADVANCEDQUEUE_STATUS_PROCESSING, array(':name' => $this->name))->fetchField();
    return db_query('SELECT count(item_id) FROM {voipqueue_calls} vqc LEFT JOIN {voipcall} vc ON vqc.cid = vc.cid WHERE vc.status IN (:status)', array(':status' => array(VoipCall::IN_PROGRESS,VoipCall::RINGING)))->fetchField();
  }

  /**
   * Retrieve the number of items queue with delayed start.
   *
   * @return int
   *   An integer estimate of the number of items in the queues.
   */
  public function numberOfDelayedItems() {
    return db_query('SELECT COUNT(item_id) FROM {advancedqueue} WHERE name = :name AND STATUS = ' . VOIPQUEUE_STATUS_DELAYED . ' AND expire > :time', array(':name' => $this->name, ':time' => time()))->fetchField();
  }

  /**
   * Retrieve items that have timed out.
   *
   * @return array
   *   Array of items.
   */
  public function retrieveExpiredItems() {
    $expired = array();
    //$result = db_query('SELECT data, item_id FROM {advancedqueue} WHERE name = :name AND status = ' . ADVANCEDQUEUE_STATUS_PROCESSING . ' AND expire < :time', array(':name' => $this->name, ':time' => time()));
    //@todo: this needs to be implemented differently
    $expire = time() - voipblast_defaults('voipblast_max_length');
    $result = db_query('SELECT a.data, a.item_id FROM {voipqueue_calls} vqc LEFT JOIN {voipcall} vc ON vqc.cid = vc.cid LEFT JOIN {advancedqueue} a ON vqc.item_id = a.item_id WHERE a.name = :queue_name AND vc.status = :status AND a.processed <= :expire', array(':queue_name' => 'voipblast_voice', ':status' => VoipCall::IN_PROGRESS, ':expire' => $expire));

    while ($item = $result->fetchObject()) {
      $item->data = unserialize($item->data);
      if (is_a($item->data, 'VoipQueueDial')) {
        $item->data->item_id = $item->item_id;
        $item = $item->data;
        $expired[] = $item;
      }
    }
    return $expired;
  }

  public function createQueue() {
    if ($this->name != 'voipqueue_calls') {
      // Record of queues managed by VoIP queue.
      $queues = variable_get('voipqueue_queues', array());
      if (! isset($queues[$this->name])) {
        $queues[$this->name] = $this->name;
        variable_set('voipqueue_queues', $queues);
      }
    }
  }

  public function deleteQueue() {
    $queues = variable_get('voipqueue_queues', array());
    if (isset($queues[$this->name])) {
      unset($queues[$this->name]);
      variable_set('voipqueue_queues', $queues);
    }
    variable_del('voipqueue_limit_' . $this->name);
    parent::deleteQueue();
  }
}

/**
 * VoipCall job.
 *
 * @todo abstract class?
 */
class VoipQueueJob {
  /**
   * Queue name
   *
   * @var string
   */
  public $queueName;

  /**
   * Advanced Queue Primary Key.
   *
   * @var int
   */
  public $item_id;

  /**
   * The user to which the item belongs.
   *
   * @var int
   */
  public $uid;

  /**
   * The internal ID of this call.
   *
   * Composed of DestinationNumber:CID. Stored as the title string in
   * advancedqueue.
   *
   * @var string
   */
  public $title;

  /**
   * The maximum call length in seconds.
   *
   * Overrides the lease time for the item when claimed.
   *
   * @var int.
   */
  public $leaseTime;

  /**
   * Delay in seconds before job should move into ready queue.
   *
   * @var int
   */
  public $delay;

  /**
   * Timestamp of original job creation.
   *
   * @var int
   */
  public $enqueued;

  /**
   * Maxium number of retries.
   */
  public $maxRetry;

  /**
   * Number of attempts made.
   */
  public $numberRetry;

  /**
   * Seconds to delay between retries.
   */
  public $retryDelay;

  /**
   * VoipCall::$cid for the queue.
   *
   * @var int
   */
  public $cid;

  /**
   * Call channel type VOIPQUEUE_DIAL|VOIPQUEUE_TEXT.
   *
   * @var int
   */
  public $type;

  /**
   * Create new delayed queue item.
   *
   * @param VoipCall $call
   *   Data for item.
   * @param bool $set_enqueued
   *   If the created, queued, time should be set on creation of the object.
   * @param int $type
   *   Call channel type voice or text. Values: VOIPQUEUE_DIAL|VOIPQUEUE_TEXT
   * @param array $options
   *   Retries: maxRetry and retryDelay; Delay; max_length.
   *   Easier to read using setters.
   */
  public function __construct(VoipCall $call, $set_enqueued = TRUE, $type = VOIPQUEUE_DIAL, $options = array()) {
    // Allow an unsaved call object to be passed. Save it, to get Cid.
    if (! $cid = @$call->getCid()) {
      // @error suppressed because ::getCid() doesn't check empty.
      $call->save();
      $cid = $call->getCid();
    }
    // Validate the channel type.
    if ($type != VOIPQUEUE_DIAL && $type != VOIPQUEUE_TEXT) {
      throw new InvalidArgumentException('Unknow call channel type');
    }
    $this->title = $call->getDestNumber() . ':' . $cid;
    $this->uid = $call->getUid();
    $this->type = $type;
    $this->cid = $cid;
    $this->leaseTime = isset($options['max_length']) ? $options['max_length'] : 600;
    $this->maxRetry = isset($options['retries']) ? $options['retries'] : 2;
    $this->retryDelay = isset($options['retry_delay']) ? $options['retry_delay'] : 60;
    $this->numberRetry = 0;
    $this->delay = isset($options['delay']) ? $options['delay'] : 0;
    if ($set_enqueued) {
      $this->enqueued = time();
    }
  }

  /**
   * Return the related call object.
   */
  public function getCall() {
    return VoipCall::load($this->cid);
  }

  /**
   * Set max call length.
   *
   * @param int $length
   *   Maximum length of a call before forcing hangup in seconds.
   */
  public function setMaxLength($length) {
    $this->leaseTime = $length;
  }

  /**
   * Set retry.
   *
   * @param int $number
   *   Number of times that retry attempts can be made.
   * @param int $delay
   *   Minimum seconds to wait between retries.
   */
  public function setRetry($number, $delay = 60) {
    $this->maxRetry = $number;
    $this->retryDelay = $delay;
  }

  /**
   * Set initial delay.
   *
   * @param int $delay
   *   Minimum seconds after enqueue time before job can be claimed.
   */
  public function setDelay($delay) {
    $this->delay = $delay;
  }

  /**
   * (Re)set the time for (re)entry into the queue.
   *
   * This time is used with the delay to calculate the start or retry time.
   */
  public function setEnqueueTime($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }
    $this->enqueued = time();
  }

  /**
   * If a retry call is permitted.
   *
   * @return bool
   *   TRUE if permitted.
   */
  public function retryPermitted($increment = TRUE) {
    $permitted = $this->numberRetry < $this->maxRetry;
    if ($increment) {
      $this->numberRetry++;
    }

    return $permitted;
  }

  /**
   * Calculate any delay time for the item.
   *
   * @return int
   *   Delay in seconds.
   */
  public function calculateDelay() {
    if ($this->numberRetry) {
      // A retry, use the retryDelay.
      $delay = $this->retryDelay;
    }
    else {
      // First time attempting; use initial delay.
      $delay = $this->delay;
    }
    if ($delay + $this->enqueued > time()) {
      // Delay plus enqueue time in the future. Set delay.
      $delay += $this->enqueued;
    }
    else {
      // No delay needed.
      $delay = 0;
    }
    return $delay;
  }

  /**
   * Prepare item to be inserted, or updated, in queue.
   *
   * @param string $queue_name
   *   The name of the queue for the item to be inserted, updated, in.
   *
   * @return array
   *   Keyed array of fields and values for database.
   */
  public function prepare($queue_name) {
    $this->queueName = $queue_name;
    if (empty ($this->enqueued)) {
      $this->enqueued = time();
    }
    $delay = $this->calculateDelay();
    return array(
      'name' => $this->queueName,
      'uid' => $this->uid,
      'title' => $this->title,
      'data' => serialize($this),
      'created' => $this->enqueued,
      'status' => $delay ? VOIPQUEUE_STATUS_DELAYED :   ADVANCEDQUEUE_STATUS_QUEUED,
      'expire' => $delay,
    );
  }
}

/**
 * Job to dial a call.
 *
 * @todo should some of the retry logic/timing be moved to this class only?
 */
class VoipQueueDial extends VoipQueueJob {
  /**
   * Create new delayed queue item.
   *
   * @param VoipCall $call
   *   Data for item.
   * @param bool $set_enqueued
   *   If the created, queued, time should be set on creation of the object.
   * @param array $options
   *   Array of other options. Use setters.
   */
  public function __construct(VoipCall $call, $set_enqueued = TRUE, $options = array()) {
    return parent::__construct($call, $set_enqueued, VOIPQUEUE_DIAL, $options);
  }
}

/**
 * Job to send a text.
 */
class VoipQueueText extends VoipQueueJob {
  /**
   * Text of message to be sent.
   *
   * @var string
   */
  public $text;

  /**
   * Create new delayed queue item.
   *
   * @param VoipCall $call
   *   Data for item.
   * @param string $text
   *   Text of message to be sent.
   * @param bool $set_enqueued
   *   If the created, queued, time should be set on creation of the object.
   * @param array $options
   *   Options, use setters.
   */
  public function __construct(VoipCall $call, $text, $set_enqueued = TRUE, $options = array()) {
    $this->text = $text;
    $options['maxRetry'] = isset($options['maxRetry']) ? $options['maxRetry'] : 0;
    return parent::__construct($call, $set_enqueued, VOIPQUEUE_TEXT, $options);
  }
}
