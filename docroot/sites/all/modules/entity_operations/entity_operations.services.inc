<?php
/**
 * @file entity_operations.services.inc
 * Defines resources for Services. Requires services_entity module.
 */

/**
 * Implements hook_services_resources().
 */
function entity_operations_services_resources() {
  // This requires services_entity module, as we add to the resources it
  // defines generically for all entity types.
  if (!module_exists('services_entity')) {
    return;
  }

  $resources = array();
  $entities_operations_info = entity_operations_get_operation_info();
  foreach ($entities_operations_info as $entity_type => $entity_operations_info) {
    foreach ($entity_operations_info as $operation_name => $operation) {
      // Get the handler and the operation info.
      $handler_class = $operation['handler'];
      $operation_handler = new $handler_class($entity_type, $operation_name);
      $operation_info = $operation_handler->operationInfo();

      if (empty($operation_info['provides services resource'])) {
        // Skip operations that don't declare that they provide a resource.
        continue;
      }

      $resources['entity_' . $entity_type]['targeted_actions']['operation_' . $operation_name] = array(
        'help' => 'Operation ' . $operation_name,
        'file' => array(
          'type' => 'inc',
          'module' => 'entity_operations',
          'name' => 'includes/entity_operations.resource',
        ),
        'callback' => 'entity_operations_services_resource_callback',
        'access callback' => 'entity_operations_services_resource_access_callback',
        'access arguments append' => TRUE,
        'args' => array(
          array(
            // This is a hardcoded parameter to our callback, which requires
            // a bit of hoop-jumping to work with Services.
            // Currently also requires a patch to Services to prevent a
            // warning. See http://drupal.org/node/1931080.
            'name' => 'entity_type',
            // We have to set this as optional so the default value gets used.
            // @see RESTServer::getControllerArguments()
            'optional' => TRUE,
            // We don't set the source.
            //'source' => NULL,
            'type' => 'string',
            'description' => 'The entity type this operation is on.',
            // The value we actually want to always pass.
            'default value' => $entity_type,
          ),
          array(
            'name' => 'handler_class',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The handler class for the operation.',
            'default value' => $handler_class,
          ),
          array(
            'name' => 'operation_path',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The operation path for the operation.',
            'default value' => $operation_name,
          ),
          array(
            'name' => 'entity_id',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to attach a file to',
          ),
        ),
      );

      // Add further parameters that the handler's execute() method expects.
      // These all come from the incoming data array.
      // This is more complicated than needs be, as we could simply pass the
      // entire incoming data array wholesale, but this way has the advantage
      // that the user of the resource gets better error messages when
      // parameters are missing.
      foreach ($operation_info['parameters'] as $parameter_name => $type) {
        $resources['entity_' . $entity_type]['targeted_actions']['operation_' . $operation_name]['args'][] = array(
          'name' => $parameter_name,
          'optional' => FALSE,
          'source' => array('data' => $parameter_name),
          'type' => $type,
          'description' => 'A parameter for the operation execute() method.',
        );
      }
    }
  }

  return $resources;
}
