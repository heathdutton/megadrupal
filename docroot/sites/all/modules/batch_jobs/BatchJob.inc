<?php

/**
 * @file
 * A class for constructing batch jobs.
 */

/**
 * BatchJob class
 */
class BatchJob {

  public $bid;

  /**
   * BatchJob construction function.
   *
   * Creates a new batch job object with a batch ID.
   *
   * @param string $title
   *   Title of the batch job.
   * @param int $uid
   *   User ID to which the batch job belongs; 0, for accessible to all users.
   */
  public function __construct($title, $uid = 0) {
    $bid = db_insert('batch_jobs')
      ->fields(array(
        'bid' => NULL,
        'title' => $title,
        'uid' => $uid,
      ))
      ->execute();
    $this->bid = $bid;
  }

  /**
   * Add batch parameters.
   *
   * @param array $params
   *   Array of batch parameters to pass to all tasks.
   */
  public function addBatchParams($params) {
    db_update('batch_jobs')
      ->fields(array(
        'data' => serialize($params),
      ))
      ->condition('bid', $this->bid)
      ->execute();
  }

  /**
   * Add callbacks to run at the end of the batch job.
   *
   * @param array $callbacks
   *   Array of functions to run at the end of batch job.
   */
  public function addBatchCallbacks($callbacks) {
    db_update('batch_jobs')
      ->fields(array(
        'callbacks' => serialize($callbacks),
      ))
      ->condition('bid', $this->bid)
      ->execute();
  }

  /**
   * Retrieve a token for the batch job.
   */
  public function getToken() {
    return drupal_get_token($this->bid);
  }

  /**
   * Add task to batch job.
   *
   * @param string $title
   *   Title of the task.
   * @param string $functions
   *   Array of callbacks to run for the task.
   * @param array $params
   *   Array of parameters to pass to the task functions.
   */
  public function addTask($title, $functions, $params = array()) {
    $tid = db_insert('batch_task')
      ->fields(array(
        'tid' => NULL,
        'bid' => $this->bid,
        'title' => $title,
        'callbacks' => serialize($functions),
        'data' => serialize($params),
      ))
      ->execute();
    return $tid;
  }
}
