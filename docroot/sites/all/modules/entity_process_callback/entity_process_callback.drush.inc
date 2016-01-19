<?php

/**
 * @file
 * Drush implementation file.
 */

define('MAX_DISPLAY_IDS', 20);

/**
 * Implements hook_drush_command().
 */
function entity_process_callback_drush_command() {
  $items = array();

  $items['entity-process-callback'] = array(
    'aliases' => array('epc', 'ep-callback'),
    'description' => 'Processes entities and applies a callback function to each of them.',
    'arguments' => array(
      'entity-type' => array(
        'description' => 'Process all the entities of this type.',
        'example-value' => 'node',
        'required' => TRUE,
      ),
      'callback' => array(
        'description' => 'Callable name that will be executed for every selected entity. It takes the entity type as the first argument and the entity as the second. Return TRUE for success or FALSE for error.',
        'example-value' => array('\MyClass::addDefaultValueForNewField'),
        'required' => TRUE,
      ),
    ),
    'options' => array(
      'ids' => array(
        'description' => 'Supply a comma-separated list of entity ids to process. If empty all entities are assumed.',
        'example-value' => '1,4,66',
      ),
      'bundles' => array(
        'description' => 'Limit the entities to the list of bundles, separated by commas. Ignored when ids is populated.',
        'example-value' => 'article,page',
      ),
      'fields' => array(
        'description' => 'Limit the entities to field conditions, pipe-delimted, separated by commas. Ignored when ids is populated.',
        'example-value' => 'field_category|tid|555,field_name|value|%sam%|like',
      ),
      'size' => array(
        'description' => 'Number of entities to populate per run. Defaults to 10.',
        'example-value' => '10',
      ),
      'queue' => array(
        'description' => 'Put the items in a queue to be processed asynchronously.',
      ),
      'efq-class' => array(
        'description' => '(ADVANCED) If you are making use of a custom EntityFieldQuery class, provide the class name here. Defaults to \\EntityFieldQuery',
        'example-value' => '\\Drupal\\my_module\\Util\\EntityFieldQuery',
      ),
    ),
    'examples' => array(
      'drush epc node \MyClass::addDefaultValueForNewField --size=25' => 'Will process all nodes.',
      'drush epc node my_module_custom_callback --ids=12,56' => 'Will process nodes with nid 12 and 56.',
      'drush epc node entity_save --bundles=article' => 'This will re-save all the articles.',
      'drush epc node entity_save --bundles=article --queue' => 'This will re-save all the articles asynchronously (putting the save operations into a queue).',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function entity_process_callback_drush_help($section) {
  if ($section != 'drush:entity-process-callback') {
    return NULL;
  }
  return dt('This command will process the selected entities with the provided callback.');
}

/**
 * Generates default users and keys.
 *
 * @param string $entity_type
 *   The type of entity
 * @param string $callable
 *   A PHP callable.
 * @param array $options
 *   The array of options.
 */
function drush_entity_process_callback($entity_type = NULL, $callable = NULL, array $options = array()) {
  try {
    // Get all the processed options.
    list($ids, $chunk_size) = drush_entity_process_callback_process_options($entity_type, $options);
  }
  catch (\EpcException $e) {
    drush_set_error('Invalid operation', dt($e->getMessage()));
    return;
  }

  // If the queue is selected then just schedule the operations.
  if (drush_get_option('queue')) {
    /* @var \SystemQueue $queue */
    $queue = DrupalQueue::get(EPC_PROCESS_QUEUE_NAME);
    // There is no harm in trying to recreate existing.
    $queue->createQueue();

    foreach ($ids as $id) {
      $queue->createItem(new \EpcOperation($entity_type, $id, $callable));
    }
    drush_log(dt('All entities have been queued.'), 'ok');
    return;
  }

  if (drush_get_error()) {
    return;
  }

  if (!drush_entity_process_callback_print_header($entity_type, $ids)) {
    drush_log('Aborted.', 'warning');
    return;
  }

  $total = count($ids);
  $chunks = array_chunk($ids, $chunk_size);
  $progress = 0;
  $operations = array();
  foreach ($chunks as $chunk) {
    $progress += count($chunk);
    $operations[] = array('drush_entity_process_callback_send_batch', array(
      $chunk,
      $entity_type,
      $callable,
      dt('@percent% (Processing @progress of @total)', array(
        '@percent' => round(100 * $progress / $total),
        '@progress' => $progress,
        '@total' => $total,
      ))),
    );
  }
  $batch = array(
    'operations' => $operations,
    'title' => dt('Entity process callback batch'),
    'finished' => 'drush_entity_process_callback_process_finished',
    'progress_message' => dt('@current entities of @total were processed with @callable.', array(
      '@callable' => $callable,
    )),
  );

  // Get the batch process all ready!
  batch_set($batch);

  // Start processing the batch operations.
  drush_backend_batch_process();
}

/**
 * Process the options and select the appropriate options.
 *
 * @param string $entity_type
 *   The type of entity
 * @param string $callable
 *   A PHP callable.
 * @param array $options
 *   The array of options.
 *
 * @return array
 *   Numeric array with the processed options
 */
function drush_entity_process_callback_process_options($entity_type, array $options = array()) {
  // Get all options.
  $options += array(
    'ids' => drush_get_option('ids'),
    'bundles' => drush_get_option('bundles'),
    'fields' => drush_get_option('fields'),
    'size' => drush_get_option('size', 10),
    'efq-class' => drush_get_option('efq-class', '\\EntityFieldQuery'),
  );

  $ids = array_filter(explode(',', $options['ids']));
  $bundles = array_filter(explode(',', $options['bundles']));
  $fields = array_filter(explode(',', $options['fields']));
  $fields = array_map(function ($condition) {
    return explode('|', $condition);
  }, $fields);
  $chunk_size = $options['size'];

  // Make sure options are valid.
  if (empty($entity_type) || !$info = entity_get_info($entity_type)) {
    throw new \EpcException('Error: The entity type provided does not exist.');
  }

  // If we were provided IDs, then we're done here.
  if (!empty($ids)) {
    return array($ids, $chunk_size);
  }

  // Validate bundles.
  if (!empty($bundles)) {
    foreach ($bundles as $bundle) {
      if (empty($info['bundles'][$bundle])) {
        throw new \EpcException('Error: The provided bundle does not exist for this entity type.');
      }
    }
  }

  $efq_class = $options['efq-class'];
  if (!class_exists($efq_class)) {
    // Make sure that the class exists.
    throw new \EpcException('Error: The entity field query class provided does not exist.');
  }
  $query = new $efq_class();
  if (!$query instanceof \EntityFieldQuery) {
    // Make sure that the class is EntityFieldQuery or a descendant of it.
    throw new \EpcException('Error: The entity field query class provided is not usable.');
  }
  $query->entityCondition('entity_type', $entity_type);
  if (!empty($bundles)) {
    $query->entityCondition('bundle', $bundles, 'IN');
  }
  if (!empty($fields)) {
    foreach ($fields as $condition) {
      call_user_func_array(array($query, 'fieldCondition'), $condition);
    }
  }
  $results = $query->execute();

  if (empty($results[$entity_type])) {
    throw new \EpcException('No entities to process');
  }

  $ids = array_keys($results[$entity_type]);
  return array($ids, $chunk_size);
}

/**
 * Prints an informational header about what is going to be synced.
 *
 * @param string $entity_type
 *   The passed in entity type.
 * @param array $ids
 *   The passed in ids.
 *
 * @return bool
 *   TRUE if the user wants to proceed. FALSE otherwise.
 */
function drush_entity_process_callback_print_header($entity_type, $ids) {
  // Tell the user what is going to be synced.
  drush_log(dt('=========================================='), 'ok');
  drush_log(dt(' The following entities will be processed'), 'ok');
  drush_log(dt('=========================================='), 'ok');
  $total = count($ids);
  if ($total > MAX_DISPLAY_IDS) {
    $id_display = dt('@ids ... and @count more.', array(
      '@ids' => implode(', ', array_slice($ids, 0, MAX_DISPLAY_IDS)),
      '@count' => $total - MAX_DISPLAY_IDS,
    ));
  }
  else {
    $id_display = implode(', ', $ids);
  }
  drush_log(dt("@count @type entities will be processed.\nWith ids: @ids", array(
    '@type' => $entity_type,
    '@count' => $total,
    '@ids' => $id_display,
  )), 'ok');
  drush_log(dt('==========================================' . PHP_EOL), 'ok');
  return drush_confirm(dt('@total entities will be batch processed. Do you want to continue?', array('@total' => $total)));
}

/**
 * Operation function for the batch processing.
 *
 * @param array $ids
 *   Array of entity ids to send in this batch.
 * @param string $entity_type
 *   They entity type.
 * @param callable $callable
 *   The callback to execute.
 * @param string $details
 *   Feedback details.
 * @param array $context
 *   Context array for batch processing.
 */
function drush_entity_process_callback_send_batch(array $ids, $entity_type, $callable, $details, &$context) {
  $context['message'] = $details;
  $entities = entity_load($entity_type, $ids);
  // Make sure to only initialize the results once.
  if (!isset($context['results']['success'])) {
    $context['results']['success'] = $context['results']['error'] = 0;
  }
  foreach ($entities as $entity) {
    try {
      $operation = new \EpcOperation($entity_type, $entity, $callable);
    }
    catch (\EpcException $e) {
      $context['results']['error']++;
      watchdog_exception('entity_process_callback', $e);
      continue;
    }
    $operation->execute() ? $context['results']['success']++ : $context['results']['error']++;
  }
}

/**
 * Finished callback.
 */
function drush_entity_process_callback_process_finished($success, $results, $operations) {
  if ($success) {
    // Let the user know we have finished.
    drush_log(dt('Finished processing.' . PHP_EOL . '@success entities have been successfully imported, @error entities failed.', array(
      '@success' => empty($results['success']) ? 0 : $results['success'],
      '@error' => empty($results['error']) ? 0 : $results['error'],
    )), 'ok');
  }
}
