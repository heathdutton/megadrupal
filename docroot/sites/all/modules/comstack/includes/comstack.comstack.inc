<?php
/**
 * @file
 * comstack.comstack.inc
 */

/**
 * Process and send a message.
 *
 * @param $message
 *   The message entity being used for the notification.
 * @param $options
 *   Array of options to override the plugin's default ones.
 * @param $notifier_name
 *   Optional; The name of the notifier to use. Defaults to "email"
 *   sending method.
 *
 * @return
 *   Boolean value denoting success or failure of the notification.
 */
function comstack_send_message(Message $message, array $options = array(), $notifiers = array('email')) {
  if (!$plugin = message_notify_get_notifier($notifier_name)) {
   throw new MessageNotifyException(format_string('Could not send notification using the "@notifier" notifier.'), array('@notifier' => $notifier_name));
  }

  $plugin['options'] = drupal_array_merge_deep($plugin['options'], $options);

  $class = ctools_plugin_load_class('message_notify', 'notifier', $notifier_name, 'class');
  $notifier = new $class($plugin, $message);
  if ($notifier->access()) {
    return $notifier->send();
  }
}

/**
 * Carry out a single operation against a series of entities, if the number of
 * entities is greater than a threshold then batch process them.
 *
 * @param string $entity_type
 *   A string defining the type of entity being processed.
 * @param string $bundle
 *   A string defining the bundle of the entity type.
 * @param array $entities
 *   An array of entity IDs.
 * @param string $op
 *   The operation to carry out against the entities, either "update" or
 *   "delete".
 * @param array $properties
 *   An array of properties on the entity to change, key is the property and
 *   the value of the array item is what that entity key will be set to. If the
 *   op is reassign this function will expect a property with the key "uid"
 *   which is an integer.
 * @param string $batch_title
 *   The title of the batch operation.
 */
function comstack_mass_update($entity_type, $bundle, $entities, $op, $properties = array(), $batch_title = NULL) {
  /**
   * Don't allow unrecognised operations.
   */
  if (!in_array($op, array('update', 'delete'))) {
    return FALSE;
  }

  if (count($entities) > variable_get('comstack_mass_update_batch_threshold', 10)) {
    $batch_title = $batch_title == '' ? t('Processing') : $batch_title;
    $batch = array(
      'operations' => array(
        array('_comstack_mass_update_batch_process', array($entity_type, $bundle, $entities, $op, $properties))
      ),
      'finished' => '_comstack_mass_update_batch_finished',
      'title' => $batch_title,
      // We use a single multi-pass operation, so the default
      // 'Remaining x of y operations' message will be confusing here.
      'progress_message' => '',
      'error_message' => t('The update has encountered an error.'),
    );
    batch_set($batch);
  }
  else {
    if (!empty($entities) && is_array($entities)) {
      foreach ($entities as $id) {
        _comstack_mass_update_helper($entity_type, $bundle, $id, $op, $properties);
      }
    }
  }
}

/**
 * Perform an operation against an entity, typically used in batch operations
 * but can be used by itself.
 *
 * @param string $entity_type
 *   A string defining the type of entity being processed.
 * @param string $bundle
 *   A string defining the bundle of the entity type.
 * @param array $entities
 *   An array of entity IDs.
 * @param string $op
 *   The operation to carry out against the entities, either "update" or
 *   "delete".
 * @param array $properties
 *   An array of properties on the entity to change, key is the property and
 *   the value of the array item is what that entity key will be set to.
 */
function _comstack_mass_update_helper($entity_type, $bundle, $entity_id, $op, $properties) {
  switch ($op) {
    case 'update':
      $wrapper = entity_metadata_wrapper($entity_type, $entity_id);

      if (!$wrapper) {
        return FALSE;
      }

      foreach ($properties as $name => $value) {
        $wrapper->{$name}->set($value);
      }

      $wrapper->save();
      break;

    case 'delete':
      try {
        $wrapper = entity_metadata_wrapper($entity_type, $entity_id);
        $wrapper->delete();
      }
      catch (EntityMetadataWrapperException $e) {
        $delete_function = "{$entity_type}_delete";
        if (function_exists($delete_function)) {
          $delete_function($entity_id);
        }
        else {
          return FALSE;
        }
      }

      break;
  }

  return TRUE;
}

/**
 * Executes a batch operation for comstack_mass_update().
 *
 * @param string $entity_type
 *   A string defining the type of entity being processed.
 * @param array $entities
 *   An array of entity IDs.
 * @param array $updates
 *   Associative array of updates.
 * @param array $context
 *   An array of contextual key/values.
 */
function _comstack_mass_update_batch_process($entity_type, $bundle, $entities, $op, $properties, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($entities);
    $context['sandbox']['entities'] = $entities;
  }

  // Process entities by groups of 5.
  $count = min(5, count($context['sandbox']['entities']));
  for ($i = 1; $i <= $count; $i++) {
    $entity_id = array_shift($context['sandbox']['entities']);
    $entity = _comstack_mass_update_helper($entity_type, $bundle, $entity_id, $op, $properties);

    // Store result for post-processing in the finished callback.
    $uri = entity_uri($entity_type, $entity);
    $context['results'][] = l($entity->title, $uri);

    // Update our progress information.
    $context['sandbox']['progress']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Menu callback: Reports the status of batch operation for comstack_mass_update().
 *
 * @param bool $success
 *   A boolean indicating whether the batch mass update operation successfully
 *   concluded.
 * @param int $results
 *   The number of nodes updated via the batch mode process.
 * @param array $operations
 *   An array of function calls (not used in this function).
 */
function _comstack_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    //drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed:');
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
}
