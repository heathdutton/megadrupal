<?php

/**
 * @file
 * Class that defines operation on MaPS Object's property.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Property;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

/**
 *
 */
abstract class Property extends PropertyWrapper {

  /**
   * @inheritdoc
   */
  public function getKey() {
    return 'property:' . $this->id;
  }

  /**
   * @inheritdoc
   */
  public function getGroupLabel() {
    return t('Property');
  }

  /**
   * @inheritdoc
   */
  public function getTranslatedTitle() {
    return maps_suite_t($this->title);
  }

  /**
   * @inheritdoc
   */
  public function extractValues(EntityInterface $entity, $options = array(), ConverterInterface $currentConverter) {
    $properties = $entity->getRelatedEntities();
    $properties = reset($properties);

    return (array_key_exists($this->id, $properties)) ? array($this->processValues($properties[$this->id], $entity, $currentConverter)) : NULL;
  }

}
