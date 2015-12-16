<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\DisabledSensorException.
 */

namespace Drupal\monitoring\Sensor;

/**
 * Thrown if a disabled sensor is requested to be executed.
 */
class DisabledSensorException extends \InvalidArgumentException {

}
