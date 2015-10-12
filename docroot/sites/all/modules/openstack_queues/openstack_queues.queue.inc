<?php

/**
 * @file
 * Contains OpenstackQueuesQueue class implementing DrupalQueueInterface
 */

class OpenstackQueuesQueue implements DrupalQueueInterface {

  /**
   * Start working with a queue.
   *
   * @param string $name
   *   The name of the queue to work with.
   */
  public function __construct($name) {
    if (openstack_queues_loaded()) {
      $this->params = openstack_queues_get_queue_options($name);
      // Override name if necessary.
      $name = isset($this->params['queue']) ? $this->params['queue'] : $name;
      $this->name = isset($this->params['prefix']) ? $this->params['prefix'] . '_' . $name : $name;
      $this->name = preg_replace("/[^\w]/", "_", $this->name);
      $this->connect();
    }
    else {
      watchdog('OpenstackQueues', 'PHP-Opencloud library is not loaded.', array(), WATCHDOG_ERROR);
    }
  }

  /**
   * Create a queue.
   */
  public function createQueue() {
    $this->queue = $this->service->createQueue($this->name);
    if (!empty($this->params['metadata'])) {
      $this->queue->setMetadata($this->params['metadata']);
    }
  }

  /**
   * Delete a queue.
   */
  public function deleteQueue() {
    $this->queue = $this->service->getQueue();
    $this->queue->setName($this->name);
    $this->queue->delete();
  }

  /**
   * Add a queue item and store it directly to the queue.
   *
   * @param mixed $data
   *   Arbitrary data to be associated with the new task in the queue.
   *
   * @return bool
   *   Returns TRUE if successful.
   */
  public function createItem($data) {
    $ttl = isset($this->params['ttl']) ? $this->params['ttl'] : 3600;
    return $this->queue->createMessage(array(
      'body' => json_encode($data),
      'ttl' => $ttl,
    ));
  }

  /**
   * Retrieve the number of items in the queue.
   *
   * This is intended to provide a "best guess" count of the number of items in
   * the queue. Depending on the implementation and the setup, the accuracy of
   * the results of this function may vary.
   *
   * e.g. On a busy system with a large number of consumers and items, the
   * result might only be valid for a fraction of a second and not provide an
   * accurate representation.
   *
   * @return int
   *   An integer estimate of the number of items in the queue.
   */
  public function numberOfItems() {
    $stats = $this->queue->getStats();
    return $stats->total;
  }

  /**
   * Claim an item in the queue for processing.
   *
   * @param int $lease
   *   How long the processing is expected to take, in seconds. Value must be
   *   between 60 and 43200 seconds. The default is 12 hours.
   * @param int $grace
   *   The message grace period. The server extends the lifetime of claimed
   *   messages at least as long as the claim itself, plus a specified grace
   *   period to deal with crashed workers. Value must be between 60 and 43200
   *   seconds. Default is 12 hours.
   *
   * @return object|bool
   *   An object if the item exists. FALSE if the item does not exist.
   */
  public function claimItem($lease = 43200, $grace = 43200) {
    $options = array();
    $options['ttl'] = $lease;
    $options['grace'] = $grace;

    // Drupal claims items one at a time.
    $options['limit'] = 1;

    $item = new stdClass();

    if ($messages = $this->queue->claimMessages($options)) {
      while ($message = $messages->next()) {
        $item->item_id = $message->getId();

        if (!empty($item->item_id)) {
          $item->data = json_decode($message->getBody(), TRUE);
          return $item;
        }
      }
    }

    return FALSE;
  }

  /**
   * Deletes an item from the queue.
   *
   * @param Message $item
   *   The item to be deleted.
   */
  public function deleteItem($item) {
    $this->queue->deleteMessages(array($item->item_id));
  }

  /**
   * Release an item from the queue.
   *
   * @param Message $item
   *   The item to be released.
   */
  public function releaseItem($item) {
    $claimed_item = $this->queue->getClaim($item->item_id);
    if ($claimed_item) {
      $claimed_item->delete();
    }
  }

  /**
   * Connect to a Openstack queue.
   */
  protected function connect() {
    $params = $this->params;

    $class = 'OpenCloud\\' . $params['provider'];
    if (!class_exists($class)) {
      $class = 'OpenCloud\OpenStack';
    }
    $this->connection = new $class($params['auth_url'], $params['credentials']);
    $this->service = $this->connection->queuesService($params['service'], $params['region']);

    if (isset($params['client_id'])) {
      $this->service->setClientId($params['client_id']);
    }

    if (!$this->service->hasQueue($this->name)) {
      $this->createQueue();
    }
    $this->queue = $this->service->getQueue($this->name);
  }
}
