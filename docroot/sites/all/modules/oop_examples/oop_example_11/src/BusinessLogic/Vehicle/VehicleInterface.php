<?php

/**
 * @file
 * Vehicle interface.
 */

namespace Drupal\oop_example_11\BusinessLogic\Vehicle;


use Drupal\oop_example_11\BusinessLogic\Common\ColorInterface;

/**
 * Vehicle interface.
 */
interface VehicleInterface extends ColorInterface, DriveInterface {

}
