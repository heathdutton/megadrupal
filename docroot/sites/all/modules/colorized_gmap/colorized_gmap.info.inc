<?php

/**
 * @file
 * Entitie's classes, controllers, etc.
 */

/**
 * Controller for colorized gmap entities metadata.
 */
class ColorizedGmapEntityMetadataController extends EntityDefaultMetadataController {

  /**
   * Overrides EntityDefaultMetadataController::entityPropertyInfo().
   *
   * @return array
   *   Entity properties' definition.
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['name'] = array(
      'label' => t('Name'),
      'description' => t('The name of the Map - a human-readable identifier'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'name',
    );

    $properties['created'] = array(
      'label' => t('Created'),
      'description' => t('The Unix timestamp when the Map was created.'),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'created',
    );

    $properties['changed'] = array(
      'label' => t('Changed'),
      'description' => t('The Unix timestamp when the Map was most recently saved.'),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'changed',
    );

    $properties['uid'] = array(
      'label' => t('User ID'),
      'description' => t('The user uid that created this map.'),
      'type' => 'integer',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'uid',
    );

    $properties['user'] = array(
      'label' => t('User'),
      'description' => t('The user uid that created this map.'),
      'type' => 'user',
      'schema field' => 'uid',
    );

    $properties['latitude'] = array(
      'label' => t('Latitude'),
      'description' => t('Latitude position'),
      'type' => 'float',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'latitude',
    );

    $properties['longitude'] = array(
      'label' => t('Longitude'),
      'description' => t('Longitude position'),
      'type' => 'float',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'longitude',
    );

    $properties['additional_settings'] = array(
      'label' => t('Additional settings'),
      'description' => t('A serialized array of additional map settings.'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'addsettings',
    );

    $properties['style'] = array(
      'label' => t('Map style settings'),
      'description' => t('A serialized array of map style settings.'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'style',
    );

    $properties['block_settings'] = array(
      'label' => t('Block settings'),
      'description' => t('Block settings'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer blocks',
      'schema field' => 'block_settings',
    );

    return $info;
  }

}

/**
 * colorized_gmap entity controller.
 */
class ColorizedGmapEntityController extends EntityAPIControllerExportable {

  /**
   * Overrides EntityAPIController::create().
   *
   * @param array $values
   *   Data and properties to prepopulate entity.
   *
   * @return Entity
   *   Entity object to create.
   */
  public function create(array $values = array()) {
    global $user;
    // Add values that are specific to our misc_config
    $values += array(
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }

  /**
   * Overrrides EntityAPIControllerExportable::save().
   *
   * @param Entity $entity
   *   Entity object to save.
   */
  /**
   * Overridden to care exportables that are overridden.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Preload $entity->original by name key if necessary.
    if (!empty($entity->{$this->nameKey}) && empty($entity->{$this->idKey}) && !isset($entity->original)) {
      $entity->original = entity_load_unchanged($this->entityType, $entity->{$this->nameKey});
    }
    // Update the status for entities getting overridden.
    if (entity_has_status($this->entityType, $entity, ENTITY_IN_CODE) && empty($entity->is_rebuild)) {
      $entity->{$this->statusKey} |= ENTITY_CUSTOM;
    }
    $entity->changed = REQUEST_TIME;
    return parent::save($entity, $transaction);
  }

}
