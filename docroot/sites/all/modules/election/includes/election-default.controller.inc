<?php
/**
 * @file
 * Default entity controller for the Election module.
 *
 * @class
 * ElectionDefaultController
 * @extends
 * EntityAPIControllerExportable
 */

class ElectionDefaultController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    // Determine whether the entity is new.
    $primary_key = isset($this->entityInfo['entity keys']['id']) ? $this->entityInfo['entity keys']['id'] : 'id';
    $entity->is_new = empty($entity->$primary_key);

    // Convert timestamp fields.
    foreach ($entity as $property => $value) {
      if (substr($property, -5) == '_time' && !empty($value) && !is_numeric($value)) {
        $entity->$property = strtotime($value);
      }
    }

    // Set 'created' and 'changed' fields.
    if (empty($entity->created)) {
      $entity->created = REQUEST_TIME;
    }
    $entity->changed = REQUEST_TIME;

    parent::save($entity, $transaction);

  }

}
