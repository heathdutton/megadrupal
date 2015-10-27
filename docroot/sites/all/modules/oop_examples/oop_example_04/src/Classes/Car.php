<?php

/**
 * @file
 * Car class.
 */

namespace Drupal\oop_example_04\Classes;

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
