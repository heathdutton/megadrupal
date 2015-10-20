<?php

/**
 * @file
 * Motorcycle class.
 */

namespace Drupal\oop_example_03\Classes;

/**
 * Motorcycle class.
 */
class Motorcycle extends Vehicle {
  /**
   * Returns class description.
   */
  public function getDescription() {
    return "This is a generic motorcycle.";
  }

}
