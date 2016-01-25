<?php

/**
 * @file
 * Queue administration pages and batch operations.
 *
 * @todo Add an intermediate form for time and limit when running operations.
 */

/**
 * Queue overview form.
 */
function devcontrol_admin_queue_list_form($form, &$form_state) {

  $options = array();

  $hook = 'cron_queue_info';
  foreach (module_implements($hook) as $module) {

    // Proceed to list module by module to detect potential overrides.
    $queues = module_invoke($module, $hook);
    drupal_alter('cron_queue_info', $queues);

    if (empty($queues)) {
      continue;
    }

    foreach ($queues as $name => $info) {
      $queue = DrupalQueue::get($name);
      $queue->createQueue();

      $row = array();

      $row['name']      = $name;
      $row['time']      = empty($info['time']) ? '' : format_interval($info['time']);
      $row['module']    = $module;
      $row['callback']  = empty($info['worker callback']) ? '' : $info['worker callback'];
      $row['overrides'] = '-';
      $row['count']     = $queue->numberOfItems();

      if (isset($options[$name])) {
        // Module overrides an existing entry.
        $legacy_module = $row['overrides'] = $options[$name][$module];
        $options[$name . $legacy_module] = $options[$name];
      }

      $options[$name] = $row;
    }
  }

  $form['queues'] = array(
    '#type'    => 'tableselect',
    '#options' => $options,
    '#header'  => array(
      'name'      => t("Name"),
      'time'      => t("Run time"),
      'module'    => t("Module"),
      'callback'  => t("Worker callback"),
      'overrides' => t("Overrides"),
      'count'     => t("Approximative size"),
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'run' => array(
      '#type'   => 'submit',
      '#value'  => t("Run selected"),
      '#submit' => array('devcontrol_admin_queue_list_form_run_submit'),
    ),
    'empty' => array(
      '#type'   => 'submit',
      '#value'  => t("Empty selected"),
      '#submit' => array('devcontrol_admin_queue_list_form_empty_submit'),
    ),
  );

  return $form;
}

/**
 * Run selected queue submit handler.
 */
function devcontrol_admin_queue_list_form_run_submit($form, &$form_state) {

  $names = array();
  foreach ($form_state['values']['queues'] as $name => $enabled) {
    if (!empty($enabled) && $name === $enabled) {
      $names[] = $name;
    }
  }

  if (!empty($names)) {

    $batch = array(
      'title' => format_plural(
        count($form_state['values']['queues']),
        "Processing @count queue",
        "Processing @count queues"),
      'file' => drupal_get_path('module', 'devcontrol') . '/devcontrol.admin.queue.inc',
      'operations' => array(),
    );

    foreach ($names as $name) {
      $batch['operations'][] = array('devcontrol_admin_queue_batch_process', array($name));
    }

    batch_set($batch);

  } else {
    drupal_set_message(t("Empty selection"), 'error');
  }
}

/**
 * Batch operation that processes a single queue.
 */
function devcontrol_admin_queue_batch_process($name, &$context) {

  // Ensure queue and callback exist.
  $queues = module_invoke_all('cron_queue_info');
  drupal_alter('cron_queue_info', $queues);
  if (!isset($queues[$name])) {
    drupal_set_message(t("Queue @queue does not exist", array('@queue'    => $name)), 'error');
    $context['finished'] = 1;
    return;
  }
  $function = $queues[$name]['worker callback'];
  if (!is_callable($function)) {
    drupal_set_message(t("Queue @queue has a wrong worker callback: @callback", array('@queue' => $name, '@callback' => $function)), 'error');
    $context['finished'] = 1;
    return;
  }

  $queue = DrupalQueue::get($name);
  $queue->createQueue();

  if (!isset($context['sandbox']['total'])) {
    $total = $queue->numberOfItems();

    // Count is not reliable on queues.
    if ($total < 1) {
      $total = 1;
    }

    $context['sandbox']['total']  = $total;
    $context['sandbox']['progress'] = 0;
  }

  // Run for 10 secondes. Prey for the queue operations to be short
  // enought to run fast.
  $max_time = time() + 10;
  $total = $context['sandbox']['total'] + 1;
  $done = false;

  do {
    try {

      if (!$item = $queue->claimItem()) {
        // We reached the end of the queue.
        // Mark as finished
        $context['finished'] = 1;
        return;
      }

      call_user_func($function, $item->data);
      $queue->deleteItem($item);

      // Total is still no reliable.
      if ($context['sandbox']['progress'] < $total) {
        $context['sandbox']['progress'] += 1;
      }

      if ($max_time < time()) {
        $done = true;
      }
    } catch (Exception $e) {
      // Something went wrong, just avoid the framework to crash.
      watchdog_exception('devcontrol_batch', $e);
      drupal_set_message(t("An error happened: @error", array('@error' => $e->getMessage())), 'error');

      // Mark as finished.
      $context['finished'] = 1;
      return;
    }
  } while (!$done);

  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['total'];
  // When dealing with very high number of items, the finished float can reach
  // a value very close to 1 but not exactly 1: PHP float conversion will make
  // being 1 and the batch will stop.
  if (0.99999 < $context['finished']) {
    $context['finished'] = 0.99;
  }
}

/**
 * Empty selected queue submit handler.
 */
function devcontrol_admin_queue_list_form_empty_submit($form, &$form_state) {
  // @todo
}

/**
 * Single queue form.
 */
function devcontrol_admin_queue_form($form, &$form_state) {
  // @todo
  return $form;
}
