<?php
/**
 * @file
 * Batch function for the Entity Recur module.
 */

/**
 * Start the batch to generate recurring entities
 */
function entity_recur_entity_batch_start($entity_type, $entity, $start_dates, $end_dates = array()) {
  // Generate the batch operations
  $operations = array();
  if ($start_dates) {
    foreach ($start_dates as $key => $start_date) {
      $operations[] = array('entity_recur_entity_batch_create_entity', array($entity_type, $entity, $start_date, isset($end_dates[$key]) ? $end_dates[$key] : NULL));
    }
  }

  // Generate the batch
  $batch = array(
    'title' => t('Generating items'),
    'operations' => $operations,
    'finished' => 'entity_recur_entity_batch_finished',
    'file' => drupal_get_path('module', 'entity_recur') . '/entity_recur.batch.inc',
  );

  batch_set($batch);
}

/**
 * Create the recurring entitys
 */
function entity_recur_entity_batch_create_entity($entity_type, $entity, $start_date, $end_date = NULL, $context) {
  if (!isset($context['results']['source'])) {
    $context['results']['source'] = array('entity_type' => $entity_type, 'entity' => $entity);
  }
  // Set the progress message
  $context['message'] = t('Date: !date', array('!date' => format_date($start_date)));
  // Create the entity.
  entity_recur_entity_create_entity($entity_type, $entity, $start_date, $end_date);
  // Store the start dates
  $context['results'][] = $start_date;
}

/**
 * Batch ending
 */
function entity_recur_entity_batch_finished($success, $results) {
  $source = FALSE;
  if (isset($results['source'])) {
    $source = $results['source'];
    unset($results['source']);
  }
  if ($success) {
    $count = count($results);
    drupal_set_message(t('%count %items have been generated.', array('%count' => $count, '%items' => format_plural($count, 'item', 'items'))));
  }
  else {
    drupal_set_message(t('An error occurred. Please try again.'), 'error');
  }

  // Determine where to redirect to
  $path = NULL;
  if ($source) {
    $uri = entity_uri($source['entity_type'], $source['entity']);
    $path = $uri['path'];
  }
  // Allow modules to alter the path
  drupal_alter('entity_recur_batch_redirect', $path, $source['entity_type'], $source['entity']);
  if ($path) {
    drupal_goto($path);
  }
}
