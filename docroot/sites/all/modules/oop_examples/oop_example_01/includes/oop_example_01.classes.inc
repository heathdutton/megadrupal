<?php

/**
 * @file
 * Example classes.
 */

/**
 * Vehicle class.
 */
class oop_example_01_vehicle {
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
class oop_example_01_car extends oop_example_01_vehicle {
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
class oop_example_01_motorcycle extends oop_example_01_vehicle {
  /**
   * Returns class description.
   */
  public function getDescription() {
    return "This is a generic motorcycle.";
  }

}
