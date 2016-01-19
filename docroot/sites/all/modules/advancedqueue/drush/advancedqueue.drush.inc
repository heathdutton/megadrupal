<?php

/**
 * @file
 * Drush worker for Advanced-queue.
 */

/**
 * Implements hook_drush_command().
 */
function advancedqueue_drush_command() {
  $items = array();
  $items['advancedqueue-process-queue'] = array(
    'description' => 'Run a processing job for a queue.',
    'arguments' => array(
      'queue' => dt('The name of the queue to process.'),
    ),
    'options' => array(
      'timeout' => 'The maximum execution time of the script. Be warned that this is a rough estimate as the time is only checked between two items.',
      'all' => 'Process all queues.',
    ),
    'aliases' => array('advancedqueue'),
  );
  $items['advancedqueue-list'] = array(
    'description' => 'Returns a list of all defined queues',
    'options' => array(
      'pipe' => 'Return a comma delimited list of queues.'
    )
  );
  return $items;
}

/**
 * Command callback for drush advancedqueue-process-queue.
 *
 * @param $queue_name
 *   Arbitrary string. The name of the queue to work with.
 */
function drush_advancedqueue_process_queue($queue = NULL) {
  // Load information about the registred queues, and sort them by weight.
  if (!$queues_info = advancedqueue_get_queues_info()) {
    return drush_set_error(dt('No queues exist.'));
  }

  $all_option = drush_get_option('all');
  if (!$all_option && empty($queue)) {
    return drush_set_error(dt('You have to specify either a set of queues or the --all parameter.'));
  }

  if ($all_option) {
    $queues = $queues_info;
  }
  else {
    // Validate queues.
    $queues = drupal_map_assoc(explode(',', $queue));
    if ($invalid_queues = array_diff_key($queues, $queues_info)) {
      return drush_set_error(dt('The following queues are invalid: !queues. Aborting.', array('!queues' => implode(', ', $invalid_queues))));
    }
    $queues = array_intersect_key($queues_info, $queues);
  }

  // Run the worker for a certain period of time before killing it.
  $timeout = drush_get_option('timeout');
  $end = $timeout ? time() + $timeout : 0;

  drush_log(dt('Starting processing loop.'));

  while (!$end || time() < $end) {
    foreach ($queues as $queue_name => $queue_info) {
      // Add default values.
      $queue_info += array(
        'delete when completed' => TRUE,
        'retry after' => FALSE,
        'max attempts' => 10,
      );

      $queue = DrupalQueue::get($queue_name);

      if ($item = $queue->claimItem()) {
        advancedqueue_process_item($queue, $queue_name, $queue_info, $item, $end);
        continue 2;
      }
    }

    // No item processed in that round, let the CPU rest.
    sleep(1);
  }

  drush_log(dt('Timeout: exiting processing loop.'));
}

/**
 * Command callback for drush advancedqueue-list.
 */
function drush_advancedqueue_list() {
  $queues = drush_advancedqueue_get_queues();
  $rows = array(array('Queue', 'Items', 'Class'));
  foreach (array_keys($queues) as $name) {
    $q = DrupalQueue::get($name);
    $rows[] = array($name, $q->numberOfItems(), get_class($q));
  }

  if (drush_get_option('pipe')) {
    $pipe = array();
    array_shift($rows);
    foreach ($rows as $r) {
      $pipe[] = implode(",", $r);
    }
    drush_print_pipe($pipe);
  }
  else {
    drush_print_table($rows, TRUE);
  }

  // Return the result for backend invoke
  return $rows;
}

/**
 * Get queues defined with hook_advanced_queue_info().
 *
 * @return
 *   Array of queues indexed by name and containing queue object and number
 *   of items.
 */
function drush_advancedqueue_get_queues() {
  $queues = &drupal_static(__FUNCTION__);
  if (!isset($queues)) {
    $advanced_queues = module_invoke_all('advanced_queue_info');
    drupal_alter('advanced_queue_info', $advanced_queues);

    $queues = array();
    foreach($advanced_queues as $name => $queue) {
      $queues[$name] = array(
        'cron' => array(
          'callback' => $queue['worker callback'],
          'time' => $queue['time'],
        )
      );
    }
  }
  return $queues;
}
