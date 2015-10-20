<?php

/**
 * @file
 * Class that defines operation on MaPS Object's object attribute.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Attribute;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Mapping;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

class Object extends Attribute {

  /**
   * @inheritdoc
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
    foreach ($values as $key => $value) {
      foreach ($value as $multiple => $id) {
        $result = Mapping::getEntityIdFromMapsId('object', $id, $currentConverter->getUidScope(), $currentConverter->getPid());
        $entity_id = reset($result);
        $values[$key][$multiple] = $entity_id ?: NULL;
      }
    }

    return $values;
  }

}
