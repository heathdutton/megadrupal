<?php
/**
 * @file
 * Provides a generic entity class for SuiteCRM entities.
 */

namespace Drupal\clients_suitecrm\Entity;

/**
 * Generic entity class for suiteCRM remote entities.
 */
class SuiteCrm extends \RemoteEntity {

  /**
   * @TODO do we need a status?
   * @var bool
   */
  public $status = TRUE;

  /**
   * The related clients resource.
   *
   * @see Edit::clientsResource()
   *
   * @var \Entity
   */
  protected static $clientsResource;

  /**
   * The clients resource for this entity type.
   *
   * @return \Entity
   *   The clients resource for this entity type.
   */
  public function clientsResource() {
    if (!self::$clientsResource) {
      self::$clientsResource = clients_resource_load($this->entityType);
    }
    return self::$clientsResource;
  }

  /**
   * Specifies the default label, which is picked up by label() by default.
   */
  protected function defaultLabel() {
    if (!empty($this->entityInfo['property map']['s_name'])) {
      $wrapper = entity_metadata_wrapper($this->entityType, $this);
      return $wrapper->s_name->value();
    }
    return $this->remote_id;
  }

  /**
   * Save an entity to the remote source.
   *
   * @param array $remote_properties
   *   (Optional) An array of properties to save. Values should be names of
   *   properties which are keys in the entity info 'property map' array. Only
   *   applies when updating rather than inserting. Client connection types may
   *   ignore this.
   *
   * @see remote_entity_save()
   */
  public function remoteSave($remote_properties = array()) {
    return entity_get_controller($this->entityType)->remote_save($this, $remote_properties);
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'suitecrm/' . $this->entityType . '/' . $this->identifier());
  }

  /**
   * Checks whether a given property / field name is a local copy.
   *
   * @param string $property
   *   The name of the property to check.
   *
   * @return bool
   *   TRUE if the given property / field name is a local copy of a remote one.
   */
  public function isShadowedProperty($property) {
    $remote_property = substr($property, 4);
    return isset($this->entityInfo['property map']['s_' . $remote_property]);
  }

  /**
   * Checks whether a given property / field name is a remote property.
   *
   * @param string $property
   *   The name of the property to check.
   *
   * @return bool
   *   TRUE if the given property / field name is a remote one.
   */
  public function isRemoteProperty($property) {
    return isset($this->entityInfo['property map'][$property]);
  }
}
