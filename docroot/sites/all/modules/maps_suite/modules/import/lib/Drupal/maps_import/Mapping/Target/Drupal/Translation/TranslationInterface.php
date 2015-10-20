<?php

/**
 * @file
 * Manage translations on Drupal Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Translation;

use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapperInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface as MapsEntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Field\FieldInterface;

interface TranslationInterface {

  /**
   * Class constructor.
   */
  public function __construct(EntityInterface $entity, array $existingEntities = array());

  /**
   * Get the related entity.
   *
   * @return EntityInterface
   */
  public function getEntity();

  /**
   * Set the related entity.
   * 
   * @param EntityInterface
   *   The related entity.
   */
  public function setEntity(EntityInterface $entity);
  
  /**
   * @todo implement this
   */
  public function setValue(FieldInterface $field, PropertyWrapperInterface $property, MapsEntityInterface $mapsEntity, $required = FALSE);
  
}
