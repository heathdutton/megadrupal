<?php
/**
 * @file
 * SuiteCrmController.
 */

namespace Drupal\clients_suitecrm\Controller;

/**
 * Entity controller implementing EntityAPIControllerInterface.
 */
class SuiteCrmController extends \RemoteEntityAPIDefaultController implements \EntityAPIControllerInterface {

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function create(array $values = array()) {
    // Set default type.
    if (empty($values['type'])) {
      $values['type'] = $this->entityType;
    }
    $entity = parent::create($values);
    return $entity;
  }

  /**
   * {@inheritdoc}
   */
  public function save($entity, \DatabaseTransaction $transaction = NULL) {
    // Prevent loops.
    if (!empty($entity->is_remote_save)) {
      return parent::save($entity, $transaction);
    }

    // If this is an entirely new entity it surely needs a remote save.
    $internal_identifier = $entity->internalIdentifier();
    $entity->needs_remote_save = !empty($entity->needs_remote_save) || (empty($internal_identifier) && empty($entity->remote_id));
    // If remote handling isn't bypassed and if needs remote save isn't true yet
    // try to figure out if a remote property has changed and thus a remote save
    // is advised.
    if (empty($this->bypass_remote_retrieve) && !$entity->needs_remote_save) {
      // Load the stored entity, if any.
      if (!empty($entity->{$this->idKey}) && !isset($entity->original)) {
        // In order to properly work in case of name changes, load the original
        // entity using the id key if it is available.
        $entity->original = entity_load_unchanged($this->entityType, $entity->{$this->idKey});
      }
      // Check if this needs a remote save - of so mark it for later processing.
      $wrapper = entity_metadata_wrapper($this->entityType, $entity);
      $original_wrapper = entity_metadata_wrapper($this->entityType, $entity->original);
      $properties = $wrapper->getPropertyInfo();
      // Diff with all the remote properties.
      foreach ($this->entityInfo['property map'] as $local_property_name => $remote_property) {
        $entity->needs_remote_save = $entity->needs_remote_save || $wrapper->{$local_property_name}->raw() != $original_wrapper->{$local_property_name}->raw();
      }
      $local_properties = array_diff_key($properties, $this->entityInfo['property map']);
      foreach ($local_properties as $local_property => $property_info) {
        $entity->needs_remote_save = $entity->needs_remote_save || ($entity->isShadowedProperty($local_property) && $wrapper->{$local_property}->raw() != $original_wrapper->{$local_property}->raw());
      }
    }

    try {
      if ($entity->needs_remote_save) {
        return parent::remote_save($entity);
      }
      else {
        return parent::save($entity, $transaction);
      }
    }
    catch (\Exception $e) {
      watchdog_exception('Clients SuiteCrm', $e);
      return FALSE;
    }
  }

}
