<?php

/**
 * @file
 * Motorcycle class.
 */

namespace Drupal\oop_example_12\BusinessLogic\Vehicle\Motorcycle;

use Drupal\oop_example_12\BusinessLogic\Vehicle\Vehicle;

/**
 * Motorcycle class.
 */
class Motorcycle extends Vehicle {

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('a generic motorcycle');
    return $s;
  }

}
