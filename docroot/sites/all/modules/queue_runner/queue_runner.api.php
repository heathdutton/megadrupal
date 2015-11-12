<?php
/**
 * @file
 * Documents the Queue Runner API for developers.
 */

/**
 * This hook provides a mechanism for declaring additional log destinations.
 *
 * Options:
 *
 * - callback (required): the name of a function that will handle the logging.
 * - skip_collector_subtasks (optional): a boolean value indicating whether to
 *   skip logging of sub-tasks of collectors.
 *
 * @see hook_queue_runner_log_info_alter()
 *
 * @return array
 *   An array of log destinations with their associated settings.
 */
function hook_queue_runner_log_info() {
  return array(
    'example' => array(
      'callback' => 'queue_runner_example',
      'skip_collector_subtasks' => TRUE,
    ),
  );
}

/**
 * Provides a way to alter log destination definitions.
 *
 * @see hook_queue_runner_log_info()
 *
 * @param array $info
 *   The info array of the implemented log destination definitions.
 */
function hook_queue_runner_log_info_alter(&$info) {
  $info['dblog']['callback'] = 'my_custom_dblog_callback';
}

/**
 * This hook can be used to add or alter properties on the log entry array,
 * before it is logged.
 *
 * Properties:
 *
 * - timestamp: The Unix timestamp of when the task was run in microseconds.
 * - message: The log message using t() placeholder format.
 * - variables: The t() placeholder array.
 * - severity: An integer indicating the severity level (same as watchdog).
 * - hostname: The hostname of the server.
 * - item_id: The ID of the task that was run.
 * - coll_id: The collector ID, if the task was part of a collector.
 * - nid: A node ID that this task relates to, if any.
 * - link: A link to something related e.g., the file log for the collector.
 * - uniqid: The unique identifier of the task.
 *
 * @param array $log_entry
 *   The log entry array of the message being saved.
 */
function hook_queue_runner_log_entry_alter(&$log_entry) {
  $log_entry['custom_property'] = 'value';
}

/**
 * This hook provides the mechanism for defining the behavior of tasks in the
 * queue.
 *
 * @see hook_queue_runner_workers_alter()
 *
 * @return array
 *   An array of workers to register.
 */
function hook_queue_runner_workers() {
  return array(
    'my_module_worker' => array(
      'callback' => 'my_module_worker_callback',
      'includes' => array(
        'file_extension' => 'inc',
        'module' => 'my_module',
        'file' => 'my_module.tasks',
      ),
      'finalize' => 'my_module_finalize_callback',
    ),
  );
}

/**
 * Provides a way to alter registered workers.
 *
 * @see hook_queue_runner_workers()
 *
 * @param array $workers
 *   The info array of the registered workers.
 */
function hook_queue_runner_workers_alter(&$workers) {
  $workers['my_module_worker']['callback'] = 'my_module_alternate_callback';
}

/**
 * Registers entity workers that can be run as tasks.
 *
 * This hook uses the same key/value configurations as the regular workers hook
 * above.
 *
 * @see hook_queue_runner_workers()
 * @see hook_queue_runner_entity_workers_alter()
 *
 * @param string $entity_type
 *   The type of entity.
 * @param string $bundle
 *   The entity bundle.
 *
 * @return array
 *   An array of entity worker callbacks.
 */
function hook_queue_runner_entity_workers($entity_type, $bundle) {
  if ($entity_type == 'node' && $bundle == 'page') {
    return array(
      'page_node_worker' => array(
        'callback' => 'my_module_queue_page_callback',
      ),
    );
  }
}

/**
 * Allows a module to block any tasks from being run on an entity.
 *
 * Return TRUE to abort new tasks on this entity.
 */
function hook_queue_runner_entity_blocked($entity_type, $entity_id, $data) {
  if ($entity_type == 'node') {
    $node = node_load($entity_id);
    // Silly example, don't allow anything else to touch user 1 nodes.
    return ($node->uid == 1 && $data['module'] != 'my_module');
  }
}

/**
 * Provides a way to alter entity workers.
 *
 * @see hook_queue_runner_entity_workers()
 *
 * @param array $workers
 *   An array of all registered entity workers.
 * @param string $entity_type
 *   The type of entity.
 * @param string $bundle
 *   The entity bundle type.
 */
function hook_queue_runner_entity_workers_alter(&$workers, $entity_type, $bundle) {
  if ($entity_type == 'node' && $bundle == 'page') {
    $workers['page_node_worker']['callback'] = 'my_module_alternate_queue_page_callback';
  }
}

/**
 * Example implementations:
 */

/**
 * Shows how to add a task to the normal queue.
 */
function my_module_static_adding_tasks() {
  $data['title'] = 'Task';
  $data['data'] = array('foo');
  queue_runner_add($data, 'my_module_worker_callback');

  // Now lets add an entity queue item.
  $data = array();
  $data['title'] = 'Task';
  $data['entity_id'] = 1;
  $data['entity_type'] = 'node';
  $data['entity_bundle'] = 'page';
  $data['retries'] = 5;
  queue_runner_add($data, 'my_module_queue_page_callback');
}

/**
 * The following example shows how to spawn sub-tasks as either serial or
 * parallel collectors.
 */
function my_module_static_adding_collector_child_tasks() {
  $data = array();

  for ($id = 1; $id <= 5; $id++) {
    $task = array();
    $task['data']['title'] = 'Subtask ' . $id;
    $task['data']['action'] = 'my_module_worker_callback';
    // You can also add an arbitrary worker to the list that is not registered.
    //$task['data']['worker']['callback'] = 'my_module_arbitrary_callback';
    $data['subtasks'][] = $task;
  }

  // This example uses both queues, only use one in practice!
  //
  // Using the parallel_collector allows the child tasks to run in parallel.
  queue_runner_add($data, 'parallel_collector');
  // Using the serial_collector allows the child tasks to run individually in sequence.
  queue_runner_add($data, 'serial_collector');
}

