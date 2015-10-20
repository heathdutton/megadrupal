<?php

/**
 * @file
 * Car class.
 */

namespace Drupal\oop_example_09\BusinessLogic\Vehicle\Car;

use Drupal\oop_example_09\BusinessLogic\Vehicle\Vehicle;

/**
 * Car class.
 */
class Car extends Vehicle {

  /**
   * The doors count.
   *
   * @var int
   */
  public $doors = 4;

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('a generic car');
    return $s;
  }

  /**
   * Returns doors description.
   */
  public function getDoorsDescription() {
    $s = t('This car has') . ' ';
    $s .= $this->doors;
    $s .= ' ' . t('doors.');
    return $s;
  }

}
