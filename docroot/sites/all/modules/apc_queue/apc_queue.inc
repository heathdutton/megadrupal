<?php

/**
 * Implementation of the KeyValueQueueBackend for APC.
 */
class APCQueueBackend implements KeyValueQueueBackend {
  /**
   * Decrements a numeric entry.
   *
   * @param string $key
   * @return KeyValueQueueBackend
   * The instance reference for chaining.
   */
  public function decrement($key) {
    apc_dec($key);

    return $this;
  }

  /**
   * Increments a numeric entry.
   *
   * @param string $key
   * @return KeyValueQueueBackend
   * The instance reference for chaining.
   */
  public function increment($key) {
    apc_inc($key);

    return $this;
  }

  /**
   * Stores an entry in the APC store.
   *
   * @param string $key
   * @param mixed $value
   * @return KeyValueQueueBackend
   * The instance reference for chaining.
   */
  public function store($key, $value) {
    apc_store($key, $value);

    return $this;
  }

  /**
   * Deletes an entry from the APC store.
   *
   * @param string $key
   * @return KeyValueQueueBackend
   * The instance reference for chaining.
   */
  public function delete($key) {
    apc_delete($key);

    return $this;
  }

  /**
   * Fetches an entry from the APC store.
   *
   * @param string $key
   * @return mixed
   */
  public function fetch($key) {
    return apc_fetch($key);
  }

  /**
   * Checks for the existence of a key.
   *
   * @param string $key
   * @return boolean
   */
  public function exists($key) {
    return apc_exists($key);
  }
}

/**
 * Compatibility class for APC versions older than 3.1.4
 */
class APCQueueBackendCompat extends APCQueueBackend {
  /**
   * Checks for the existence of a key.
   *
   * @param string $key
   * @return boolean
   */
  public function exists($key) {
    $success = FALSE;
    apc_fetch($key, $success);

    return $success;
  }

}

/**
 * Naive queue implementation with APC backend.
 */
class APCQueue extends NaiveKeyValueQueue {

  /**
   * Returns the queue implementation.
   *
   * @return KeyValueQueueBackend
   */
  protected function getBackend() {
    return function_exists('apc_exists') ?
      new APCQueueBackend() :
      new APCQueueBackendCompat();
  }
}
