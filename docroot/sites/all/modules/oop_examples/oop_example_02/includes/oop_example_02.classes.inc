<?php

/**
 * @file
 * Example classes.
 */

namespace OopExample02; {

  /**
   * Vehicle class.
   */
  class Vehicle {
    /**
     * Returns class description.
     */
    public function getDescription() {
      return "This is a generic vehicle.";
    }

  }

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

}
