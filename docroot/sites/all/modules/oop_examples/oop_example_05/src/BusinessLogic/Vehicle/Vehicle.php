<?php

/**
 * @file
 * Vehicle class.
 */

namespace Drupal\oop_example_05\BusinessLogic\Vehicle;

/**
 * Vehicle class.
 */
class Vehicle {
  /**
   * Returns class description.
   */
  public function getDescription() {
    return t('This is a generic vehicle.');
  }

}
