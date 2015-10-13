<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensor.
 */

namespace Drupal\monitoring\Sensor;

/**
 * Abstract SensorInterface implementation with common behaviour.
 *
 * @todo more
 */
abstract class Sensor implements SensorInterface {

  /**
   * Current sensor info object.
   *
   * @var SensorInfo
   */
  protected $info;

  /**
   * Instantiates a sensor object.
   *
   * @param SensorInfo $info
   *   Sensor info object.
   */
  function __construct(SensorInfo $info) {
    $this->info = $info;
  }

  /**
   * {@inheritdoc}
   */
  public function getSensorName() {
    return $this->info->getName();
  }

  /**
   * {@inheritdoc}
   */
  public function isEnabled() {
    return (boolean) $this->info->getSetting('enabled');
  }

}
