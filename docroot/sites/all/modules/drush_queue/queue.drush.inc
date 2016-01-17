<?php

/**
 * Implements hook_drush_help().
 */
function queue_drush_help($section) {
  switch ($section) {
    case 'drush:process-waiting-queue':
     return dt('Run the named, waiting Drupal queue. Allows for processing queues that block indefinitely waiting for new jobs without holding up other queues.');
  }
}

/**
 * Implements hook_drush_command().
 */
function queue_drush_command() {
  $items['queue-run'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'description' => 'Run a specific queue by name',
    'arguments' => array(
      'queue_name' => 'The name of the queue to run, as defined in hook_queue_info().',
    ),
  );
  $items['queue-list'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'description' => '',
    'arguments' => array(
      'queue_name' => 'The name of the queue to run, as defined in hook_queue_info().',
    ),
  );

  return $items;
}

/**
 * Queue runner that fires off of hook_queue_info() instead of
 * hook_cron_queue_info(). TODO make this as an upstream patch to
 * Drupal Queue and core.
 */
function drush_queue_run($queue_name) {
  if (!$queue_name) {
    drush_set_error('DRUSH_QUEUE_ERROR', dt('Must provide a queue name'));
  }

  // Get all queues.
  $queues = drush_queue_get_queues();

  if (isset($queues[$queue_name])) {
    if (drush_drupal_major_version() == 6) {
      drupal_queue_include();
    }

    // Try to increase the maximum execution time if it is too low.
    $max_execution_time = ini_get('max_execution_time');
    if ($max_execution_time > 0 && $max_execution_time < 240 && !ini_get('safe_mode')) {
      set_time_limit(240);
    }

    $info = $queues[$queue_name];
    $function = $info['cron']['callback'];
    $end = time() + (isset($info['cron']['time']) ? $info['cron']['time'] : 15);
    $queue = DrupalQueue::get($queue_name);
    while (time() < $end && ($item = $queue->claimItem())) {
      $function($item->data);
      $queue->deleteItem($item);
    }
  }
  else {
    drush_set_error('DRUSH_QUEUE_ERROR', dt('Could not find the !name queue.', array('!name' => $queue_name)));
  }
}

function drush_queue_list() {
  if (drush_drupal_major_version() == 6) {
    drupal_queue_include();
  }
  $queues = drush_queue_get_queues();
  $default_class = variable_get('queue_default_class', 'SystemQueue');
  $rows = array(array('Queue', 'Items', 'Class'));
  foreach (array_keys($queues) as $name) {
    $q = DrupalQueue::get($name);
    $class = variable_get('queue_class_' . $name, $default_class);
    $rows[] = array($name, $q->numberOfItems(), $class);
  }
  drush_print_table($rows, TRUE);
}

/**
 * Get queues defined with hook_cron_queue_info().
 *
 * @return Array of queues indexed by name and containing
 */
function drush_queue_get_queues() {
  static $queues;
  if (!isset($queues)) {
    // Invoke hook_queue_info().
    $queues = module_invoke_all('queue_info');

    // Merge in queues from modules that implement hook_cron_queue_info().
    $cron_queues = module_invoke_all('cron_queue_info');
    drupal_alter('cron_queue_info', $cron_queues);
    foreach($cron_queues as $name => $queue) {
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


