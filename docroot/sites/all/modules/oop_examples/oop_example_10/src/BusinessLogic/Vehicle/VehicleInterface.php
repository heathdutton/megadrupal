<?php

/**
 * @file
 * Vehicle interface.
 */

namespace Drupal\oop_example_10\BusinessLogic\Vehicle;


use Drupal\oop_example_10\BusinessLogic\Common\ColorInterface;

/**
 * Vehicle interface.
 */
interface VehicleInterface extends ColorInterface, DriveInterface {

}
