<?php

/**
 * @file
 * Class that represents a converter of type object.
 */

namespace Drupal\maps_import\Converter;

use Drupal\maps_import\Mapping\Object as MappingObject;

class Object extends Converter {

  /**
   * @inheritdoc
   *
   * Provide the code property as default value for the converter add form.
   */
  protected $uid = 'property:code';

  /**
   * @inheritdoc
   */
  public function entityInfo() {
    return array_diff_key(parent::entityInfo(), array_flip(array('file')));
  }

  /**
   * @inheritdoc
   */
  public function getMapping() {
    if (!isset($this->mapping)) {
      $this->mapping = new MappingObject($this);
    }

    return $this->mapping;
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'object';
  }

  /**
   * @inheritdoc
   */
  public function isMappingAllowed() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function hasAdditionalOptions() {
    // Check if the target entity can be unpublished.

    // When creating a new converter, the entity type is not defined...
    if ($entity_type = $this->getEntityType()) {
      // Retrieve entity information and Drupal Entity class.
      $entityInfo = entity_get_info($entity_type);
      $drupalEntityClass = (class_exists($entityInfo['maps_import_handler'])) ? $entityInfo['maps_import_handler'] : 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Entity';
      $entity = new $drupalEntityClass($this, array());
      return $entity->hasPublicationFeature();
    }

    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function optionsForm($form, &$form_state) {
    $options = $this->getOptions();

    return array(
      'status' => array(
        '#type' => 'select',
        '#title' => t('Unpublished status management'),
        '#description' => t('When MaPS objects are unpublished, choose between unpublish the Drupal entities, or delete them'),
        '#options' => array(
          'unpublish' => t('Unpublish'),
          'delete' => t('Delete'),
        ),
        '#default_value' => !empty($options['status']) ? $options['status'] : 'unpublish',
      )
    );
  }

}
