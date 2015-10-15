<?php
/**
 * @file entity_operations.resource.inc
 * Provides resource callbacks for Services. Requires services_entity module.
 */

/**
 * Services resource callback.
 *
 * This is generic, and is used for all operations.
 *
 * @param $entity_type
 *  The entity type.
 * @param $handler_class
 *  The name of the handler class for the current operation.
 * @param $operation_name
 *  The path component of the operation.
 * @param $entity
 *  The entity.
 * @param ...
 *  Additional parameters to be formed into the $parameters array for the
 *  operation handler's execute() method.
 *
 * @return
 *  The result of the operation's execute() method, or if that returns nothing,
 *  the affected entity.
 */
function entity_operations_services_resource_callback($entity_type, $handler_class, $operation_name, $entity_id) {
  $operation_handler = new $handler_class($entity_type, $operation_name);
  $operation_info = $operation_handler->operationInfo();

  $entity = entity_load_single($entity_type, $entity_id);

  // Get additional arguments to this function. These are the values that the
  // handler's execute() expects in its $parameters argument. We need to
  // determine the key names for them to build the $parameters array.
  $extra_arguments = array_slice(func_get_args(), 4);
  foreach ($operation_info['parameters'] as $parameter_name => $type) {
    $parameters[$parameter_name] = array_shift($extra_arguments);
  }

  // Operations may throw an exception: catch it and return a services error.
  try {
    $result = $operation_handler->execute($entity_type, $entity, $parameters);
  }
  catch (Exception $e) {
    return services_error($e->getMessage(), $e->getCode());
  }

  return isset($result) ? $result : $entity;
}

/**
 * Services access callback.
 *
 * This is generic, and is used for all operations.
 *
 * Parameters are the same as entity_operations_services_resource_callback().
 */
function entity_operations_services_resource_access_callback($entity_type, $handler_class, $operation_name, $entity_id) {
  $operation_handler = new $handler_class($entity_type, $operation_name);
  $entity = entity_load_single($entity_type, $entity_id);

  // Get access for the operation.
  $access = $operation_handler->access($entity_type, $entity);
  return $access;
}
