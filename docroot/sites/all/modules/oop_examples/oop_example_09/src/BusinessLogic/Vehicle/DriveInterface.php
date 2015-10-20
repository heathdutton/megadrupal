<?php

/**
 * @file
 * Drive interface.
 */

namespace Drupal\oop_example_09\BusinessLogic\Vehicle;

/**
 * Drive interface.
 */
interface DriveInterface {
  /**
   * Makes left turn.
   */
  public function turnLeft();

  /**
   * Makes right turn.
   */
  public function turnRight();

}
