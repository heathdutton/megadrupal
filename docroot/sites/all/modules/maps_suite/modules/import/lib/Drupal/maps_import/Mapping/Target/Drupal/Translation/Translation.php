<?php

/**
 * @file
 * Base class for translations management on Drupal Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Translation;

use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface as MapsEntityInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapperInterface;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Field\FieldInterface;

abstract class Translation implements TranslationInterface {

  /**
   * The entity to work on.
   *
   * @var EntityInterface
   */
  private $entity;

  /**
   * @inheritdoc
   */
  public function getEntity() {
    return $this->entity;
  }

  /**
   * @inheritdoc
   */
  public function setEntity(EntityInterface $entity) {
    $this->entity = $entity;
  }
  
  /**
   * @inheritdoc
   */
  public function setValue(FieldInterface $field, PropertyWrapperInterface $property, MapsEntityInterface $mapsEntity, $required = FALSE) {}

}
