<?php

/**
 * @file
 * Motorcycle class.
 */

namespace Drupal\oop_example_04\Classes;

/**
 * Motorcycle class.
 */
class Motorcycle extends Vehicle {
  /**
   * Returns class description.
   */
  public function getDescription() {
    return t('This is a generic motorcycle.');
  }

}
