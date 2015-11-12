<?php
/**
 * @file
 * Drupal Client for PA11Y web service.
 * See https://github.com/nature/pa11y-webservice/.
 *
 * Main wrapper around webservice.
 */

namespace lib\PA11Y\Client;


/**
 * PA11Y Client
 *
 * This is a Drupal client for the PA11Y web service.
 * See https://github.com/nature/pa11y-webservice
 * with some additional methods thrown in for good measure.
 */
class PA11YDrupalClient {

  protected $tasks;
  protected $url;

  /**
   * Constructor.
   */
  public function __construct($url) {
    $this->url = $url;

    // Loop through DB registry to reload tasks already entered.
    $task_ids = db_select('pa11y_task_registry', 'p')->fields('p', array('task', 'created'))->execute();

    foreach ($task_ids as $task_register) {
      $this->getTask($task_register);
    }
  }

  /**
   * Create task.
   *
   * @input
   * task : task to submit
   * standard: which standard to test against; i.e.:
   * WCAG2AA, WCAG2AAA, Section508
   *
   * $ignore : array of rules to exclude.
   */
  public function createTask(&$task, $standard, $ignore = array()) {

    $answer = drupal_http_request($this->url . '/tasks', array(
      'method' => "POST",
      'data' => json_encode(array(
      'url' => $task->url, 'name' => $task->name, 'standard' => $standard, 'ignore' => $ignore))));
    $return = json_decode($answer->data);

    if (!isset($return->id)) {
      throw new \Exception('No task id returned for PA11Y submission: ' . $task->url . ':' . $standard . ':' . $task->name . 'return header:' . print_r($answer, TRUE));
    }
    $task->id = $return->id;

    $this->addTask($task);

  }

  /**
   * Get all tasks.
   */
  public function getTasks() {

    return $this->tasks;
  }

  /**
   * Get full results for all tasks.
   */
  public function getTasksResults() {

    if (empty($this->tasks)) {
      drupal_set_message(t('No tasks were found, please run a <a href="/admin/config/content/pa11y_integration/settings">Test</a> first'), 'warning');
      return '';
    }

    foreach ($this->tasks as $task) {

      $task = $this->getTaskResults($task->id);
      $task->name = $this->tasks[$task->task]->name;
      $task->url = $this->tasks[$task->task]->url;
      $task->date = date('Y-m-d H:i:s', $this->tasks[$task->task]->created);
      $return[] = $task;
    }

    return $return;
  }

  /**
   * Get a particular task.
   */
  public function getTask($task_register) {

    // Get results.
    $url = $this->url . "/tasks/" . $task_register->task;
    $answer = drupal_http_request($url, array('method' => "GET"));

    $task = json_decode($answer->data);

    // Add register info.
    foreach ($task_register as $key => $value) {
      $task->$key = $value;
    }

    $this->addTask($task);

    return $task;
  }

  /**
   * Delete a tasks.
   */
  public function deleteTasks() {
    foreach ($this->tasks as $task) {

      $this->deleteTask($task->id);
    }
  }

  /**
   * Delete a tasks.
   */
  public function deleteTask($task_id) {
    $url = $this->url . '/tasks/' . $task_id;
    $answer = drupal_http_request($url, array('method' => "DELETE"));

    if ($answer->code == 204) {
      return $answer;
    }
    else {
      throw new \Exception('Could not delete task: ' . var_export($answer, TRUE));
    }
  }

  /**
   * Run a task.
   */
  public function runTask($task) {

    $return = drupal_http_request($this->url . '/tasks/' . $task->task . '/run', array('method' => "POST"));
    if ($return->code != 202) {
      throw new \Exception('run failed for task: ' . $task->task . 'with error code: ' . $return->code);
    }
    return TRUE;
  }

  /**
   * Get results for a task.
   */
  public function getTaskResults($task_id) {
    if (!$task_id || $task_id < 1) {
      throw new \Exception('No id submitted for ' . __FILE__ . ':' . __LINE__);
    }

    $url = $this->url . '/tasks/' . $task_id . '/results?full=true';
    $answer = drupal_http_request($url, array('method' => "GET"));

    $task = json_decode($answer->data);

    // If something was found it will be in an array, otherwise it won't be.
    if (isset($task[0])) {
      $task = $task[0];
    }
    else {
      $task = new \stdClass();
      $task->id = $task->task = $task_id;
      $task->date = '';
    }

    return $task;
  }

  /**
   * Get a result for a task.
   */
  public function getTaskResult($task_id, $result_id) {
    $url = $this->url . '/tasks/' . $task_id . '/results' . $result_id . '?full=true';
    $answer = drupal_http_request($url, array('method' => "GET"));
    return $answer->data;
  }

  /**
   * Add a task to the internal task registry.
   */
  protected function addTask($task) {
    if (!isset($task->id)) {
      throw new \Exception('No id found for task: ' . var_export($task, TRUE));
    }

    $this->tasks[$task->id] = $task;
  }
}
