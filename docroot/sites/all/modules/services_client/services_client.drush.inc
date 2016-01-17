<?php

/**
 * @file
 * Services client drush integration
 */

/**
 * Implementation of hook_drush_commands()
 */
function services_client_drush_command() {
  $items = array();

  $items['services-client-process-queue'] = array(
    'description' => "Process entries from queue",
    'options' => array(
      'time' => 'Number of seconds that process should run',
      'sleep' => 'Sleep time in seconds when there is nothing to process. Default is 1 second.',
      'retries' => 'Number of retires before data are sent to general error handler',
    ),
    'examples' => array(
      'drush sc-pq' => 'Process queued synchronizations',
    ),
    'aliases' => array('sc-pq')
  );

  $items['services-client-migrate-hooks'] = array(
    'description' => "Migrate old hooks to new event system",
    'options' => array(
      'hook' => 'Optionally provide hook name to limit',
    ),
    'examples' => array(
      'drush sc-mh' => "Migrate existing hooks to new event system"
    ),
    'aliases' => array('sc-mh'),
  );

  return $items;
}

/**
 * Process queued sync jobs
 */
function drush_services_client_process_queue() {
  // How long can command run
  $time = drush_get_option('time', 55);
  $end = time() + $time;
  $sleep = drush_get_option('sleep', 1);

  // Run only one process at time
  if (lock_acquire('services_client_process_queue', (float) $time)) {
    $queue = DrupalQueue::get('services_client_sync');

    // For given period of time try to fetch data form queue and process them.
    while (time() < $end) {
      if ($item = $queue->claimItem()) {
        $retries = drush_get_option('retries', 3);
        $result = services_client_queue_sync($item->data);
        if (is_object($result) && get_class($result) == 'ServicesClientEventResult') {
          while (!$result->success() && $result->error_type != ServicesClientErrorType::LOOP && $retries > 0) {
            // Retry and get new result
            $result = $result->retry();
            $retries--;
          }

          // Send result for further processing by other modules.
          module_invoke_all('services_client_process_events', array($result));
        }
        $queue->deleteItem($item);
      }
      else {
        sleep($sleep);
      }
    }
    lock_release('services_client_process_queue');
  }
}

/**
 * Migrate old system hooks to new event system.
 */
function drush_services_client_migrate_hooks() {
  module_load_include('inc', 'services_client', 'services_client.legacy');
  $name = drush_get_option('hook', NULL);
  if (!empty($name)) {
    $names = array($name);
  }
  else {
    $names = db_query("SELECT name FROM {services_client_connection_hook}")->fetchAllKeyed(0,0);
  }

  foreach ($names as $name) {
    $event = services_client_migrate_hook($name);
    ctools_export_crud_delete('services_client_connection_event', $event);
    ctools_export_crud_save('services_client_connection_event', $event);
    drush_print(dt("@name migrated.", array('@name' => $event->name)));
  }
}
