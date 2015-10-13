<?php

/**
 * @file
 * Car class.
 */

namespace Drupal\oop_example_03\Classes;

/**
 * Car class.
 */
class Car extends Vehicle {
  /**
   * Returns class description.
   */
  public function getDescription() {
    return "This is a generic car.";
  }

}
