<?php
/**
 * @file
 * Sensor Extended Info interface.
 */

namespace Drupal\monitoring\Sensor;

use Drupal\monitoring\Result\SensorResultInterface;

/**
 * Interface for a sensor with extended info.
 *
 * Implemented by sensors with verbose information.
 */
interface SensorExtendedInfoInterface {

  /**
   * Provide additional info about sensor call.
   *
   * @param SensorResultInterface $result
   *   Sensor result.
   *
   * @return string
   *   Sensor call verbose info.
   */
  function resultVerbose(SensorResultInterface $result);

}
