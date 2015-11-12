<?php

/**
 * @file
 * Toyota class.
 */

namespace Drupal\oop_example_11\BusinessLogic\Vehicle\Car\Toyota;

use Drupal\oop_example_11\BusinessLogic\Vehicle\Car\Car;

/**
 * Toyota class.
 */
class Toyota extends Car {

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('Toyota');
    return $s;
  }

}
