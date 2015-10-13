<?php

/**
 * @file
 * Vehicle class.
 */

namespace Drupal\oop_example_11\BusinessLogic\Vehicle;

/**
 * Vehicle class.
 */
class Vehicle implements VehicleInterface {

  /**
   * The vehicle color.
   *
   * @var string
   *
   * Default color translation t() is set up in class constructor because
   * expression is not allowed as field default value like:
   * public $color = t('red');
   */
  public $color;

  /**
   * Class constructor.
   */
  public function __construct() {
    $this->color = t('red');
  }

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('a generic vehicle');
    return $s;
  }

  /**
   * Returns class description.
   */
  public function getDescription() {
    $s = t('This is') . ' ';
    $s .= $this->getClassTypeDescription();
    $s .= ' ' . t('of color') . ' ';
    $s .= $this->color;
    $s .= '.';
    return $s;
  }

  /**
   * Returns color of the object.
   */
  public function getColor() {
    return $this->color;
  }

  /**
   * Makes left turn.
   */
  public function turnLeft() {
    // Turning code is omitted.
    return "Turn Left.";
  }

  /**
   * Makes right turn.
   */
  public function turnRight() {
    // Turning code is omitted.
    return "Turn Right.";
  }

  /**
   * Drives straight x km.
   */
  public function driveStraight($x) {
    // Driving code is omitted.
    return "Drive straight " . $x . " km.";
  }

}
