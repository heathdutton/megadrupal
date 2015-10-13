<?php

/**
 * @file
 * Definition of WoW\DataResource.
 */

/**
 * Defines a data resource entity class.
 */
abstract class WoWDataResource extends Entity {

  /**
   * Creates a new data resource.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    if (empty($entityType)) {
      throw new Exception('Cannot create an instance of Entity without a specified entity type.');
    }
    $this->entityType = $entityType;
    $this->setUp();
  }
}
