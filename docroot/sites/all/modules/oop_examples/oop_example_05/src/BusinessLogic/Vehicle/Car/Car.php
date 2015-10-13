<?php

/**
 * @file
 * Car class.
 */

namespace Drupal\oop_example_05\BusinessLogic\Vehicle\Car;

use Drupal\oop_example_05\BusinessLogic\Vehicle\Vehicle;

/**
 * Car class.
 */
class Car extends Vehicle {
  /**
   * Returns class description.
   */
  public function getDescription() {
    return t('This is a generic car.');
  }

}
