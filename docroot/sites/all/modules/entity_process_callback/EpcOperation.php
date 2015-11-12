<?php

/**
 * @file
 * Contains \EpcOperation.
 */

class EpcOperation {

  /**
   * Constructs an EpcOperation object.
   *
   * @param string $entity_type
   *   The entity type.
   * @param mixed $entity
   *   The entity or its ID.
   * @param string $callable
   *   The name of the callable.
   *
   * @throws \EpcException
   *   If the object cannot be constructed.
   */
  public function __construct($entity_type, $entity, $callable) {
    $this->entityType = $entity_type;
    if (is_numeric($entity)) {
      $entity = entity_load($entity_type, array($entity));
      $entity = reset($entity);
    }
    if (!$entity) {
      throw new \EpcException(sprintf('Error: Unknown entity.', $callable));
    }
    $this->entity = $entity;
    if (!is_callable($callable)) {
      throw new \EpcException(sprintf('Error: The provided callback "%s" is not callable.', $callable));
    }
    $this->callable = $callable;
  }

  /**
   * Executes the operation.
   *
   * @return bool
   *   TRUE in case of success. FALSE otherwise.
   */
  public function execute() {
    return call_user_func_array($this->callable, array($this->entityType, $this->entity));
  }

}
