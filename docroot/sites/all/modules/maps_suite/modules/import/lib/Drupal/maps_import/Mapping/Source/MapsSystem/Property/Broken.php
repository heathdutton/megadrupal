<?php

/**
 * @file
 * Broken property class.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Property;

use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

class Broken extends Property {

  /**
   * @inheritdoc
   */
  public function getValues(EntityInterface $entity) {}

  /**
   * @inheritdoc
   */
  public function getDescription(array $granularity, $separator = ' - ') {}

}
