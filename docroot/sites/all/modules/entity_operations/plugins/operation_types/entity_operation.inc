<?php

/**
 * @file
 * CTools plugin. Provides support for Entity Operations to VBO.
 */

$plugin = array(
  'title' => t('Entity operation'),
  'list callback' => 'entity_operations_vbo_entity_operations_list',
  'handler' => array(
    'file' => 'entity_operations.class.php',
    'class' => 'EntityOperationsVBOOperations',
  ),
);

/**
 * Returns a prepared list of available actions.
 *
 * Actions are fetched by invoking hook_action_info() and by loading
 * advanced actions from the database.
 *
 * @param $operation_id
 *   The full, prefixed operation_id of the operation (in this case, action)
 *   to return, or NULL to return an array with all operations.
 */
function entity_operations_vbo_entity_operations_list($operation_id = NULL) {
  $vbo_operations = &drupal_static(__FUNCTION__, array());

  // Create operations per-entity.
  $operations_info = entity_operations_get_operation_info();
  foreach ($operations_info as $entity_type => $entity_operations_info) {
    foreach ($entity_operations_info as $key => $operation_definition) {
      $handler_class = $operation_definition['handler'];
      $operation_handler = new $handler_class($entity_type, $key);
      $operation_info = $operation_handler->operationInfo();
      // Fill in a default array of VBO options for the operation.
      $operation_info += array(
        'vbo' => array(),
      );

      // Skip operations that don't provide a VBO.
      if (empty($operation_info['provides vbo'])) {
        continue;
      }

      $new_operation_id = implode('::', array('entity_operation', $entity_type, $key));
      // Start with what the operation info provides and fill in defaults.
      $vbo_operations[$new_operation_id] = $operation_info['vbo'] + array(
        'operation_type' => 'entity_operation',
        // Acts on the given entity type. This creates potentially multiple
        // VBO operations per entity operation handler, if it is in use on
        // several entity types.
        'type' => $entity_type,
        // Keep the unprefixed key around, for internal use.
        'key' => $key,
        // The handler class, so we don't have to go looking up the operations
        // info when executing the VBO op.
        'handler' => $operation_definition['handler'],
        // Use the button label for the VBO label.
        'label' => $operation_handler->getOperationString('button label', $entity_type, NULL, $key),
        'parameters' => array(),
        'configurable' => FALSE,
        'aggregate' => FALSE,
        'behavior' => array(),
        // Whole load of TODO here!
        'permissions' => NULL,
        'pass rows' => FALSE,
      );
    }
  }

  if (isset($operation_id)) {
    return isset($vbo_operations[$operation_id]) ? $vbo_operations[$operation_id] : FALSE;
  }
  else {
    return $vbo_operations;
  }
}
