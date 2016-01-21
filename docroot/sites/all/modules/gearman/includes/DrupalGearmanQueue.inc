<?php

/**
 * Integration of Gearman as backend for a DrupalQueueInterface.
 *
 * Items are submitted directly as background Gearman jobs (with normal priority). The data for an item is encoded in
 * JSON in the job workload.
 *
 * A DrupalGearmanQueue cant only be used to enqueue item. Consuming items is not supported.
 *
 */
class DrupalGearmanQueue implements DrupalQueueInterface {

  /**
   * @var string
   */
  private $name;

  /**
   * @var GearmanClient
   */
  private $client;

  /**
   * Start working with a queue.
   */
  function __construct($name) {
    $this->name = $name;
  }

  /**
   * Get the GearmanClient for this queue.
   *
   * @return GearmanClient
   */
  protected function getClient() {
    if (!isset($this->client)) {
      $this->client = new GearmanClient();
      $this->client->setTimeout(variable_get('gearman_client_timeout', 1000));
      $this->client->addServers(variable_get('gearman_server', 'localhost:4730'));
    }
    return $this->client;
  }

  /**
   * Add a queue item and store it directly to the queue.
   */
  public function createItem($data) {
    // Enqueue the item as a Gearman background job.
    $handle = $this->getClient()->doBackground($this->name, json_encode(array('data' => $data, 'created' => time())));
  }

  /**
   * Retrieve the number of items in the queue.
   */
  public function numberOfItems() {
    return 0;
  }

  /**
   * Claim an item in the queue for processing.
   *
   * Gearman does not allow claiming job in a Drupal's API compatible way. There is no way to retrieve a Gearmon job,
   * return it and then let the caller complete it (success or failure) using the Gearman PECL API. The only way to
   * retireve a job is to use GearmanWorker::work(). But then, the job has to be processed in the registered callback.
   * If the GearmanJob is stored in the returned item (using an anonymous function as worker callback), PHP will crash
   * (segfault) as soon one of its method is invoked.
   */
  public function claimItem($lease_time = 3600) {
    return FALSE;
  }

  /**
   * Delete a finished item from the queue.
   */
  public function deleteItem($item) {
    // Nothing to do here.
  }

  /**
   * Release an item that the worker could not process, so another
   * worker can come in and process it before the timeout expires.
   */
  public function releaseItem($item) {
    // Nothing to do here.
  }

  /**
   * Create a queue.
   */
  public function createQueue() {
    // Nothing to do here.
  }

  /**
   * Delete a queue and every item in the queue.
   */
  public function deleteQueue() {
    // Nothing to do here.
  }
}
