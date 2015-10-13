<?php

/**
 * @file
 * Car class.
 */

namespace Drupal\oop_example_06\BusinessLogic\Vehicle\Car;

use Drupal\oop_example_06\BusinessLogic\Vehicle\Vehicle;

/**
 * Car class.
 */
class Car extends Vehicle {

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('a generic car');
    return $s;
  }

}
