<?php

/**
 * @file
 * Class that defines operation on MaPS Object's integer attribute.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Attribute;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

class Int extends Attribute {

  /**
   * @inheritdoc
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
    foreach ($values as $key => $value) {
      foreach ($value as $multiple => $multiple_value) {
        $values[$key][$multiple] = (int) $multiple_value;
      }
    }

    return $values;
  }

}
