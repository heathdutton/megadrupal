<?php
/**
 * @file
 * Contains \Drupal\monitoring\Entity\SensorResultEntity.
 */

namespace Drupal\monitoring\Entity;

/**
 * The monitoring_sensor_result entity class.
 */
class SensorResultEntity extends \Entity {

  /**
   * The sensor name.
   *
   * @var string
   */
  public $sensor_name;

  /**
   * The sensor status.
   *
   * @var int
   */
  public $sensor_status;

  /**
   * The sensor status.
   *
   * @var string
   */
  public $sensor_value;

  /**
   * The sensor message(s).
   *
   * @var string
   */
  public $sensor_message;

  /**
   * The sensor timestamp in UNIX time format.
   *
   * @var int
   */
  public $timestamp;

  /**
   * The sensor execution time in ms.
   *
   * @var float
   */
  public $execution_time;
}
