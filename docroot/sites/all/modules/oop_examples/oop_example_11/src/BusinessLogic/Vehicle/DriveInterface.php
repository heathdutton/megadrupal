<?php

/**
 * @file
 * Drive interface.
 */

namespace Drupal\oop_example_11\BusinessLogic\Vehicle;

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

  /**
   * Drives straight x km.
   */
  public function driveStraight($x);

}
