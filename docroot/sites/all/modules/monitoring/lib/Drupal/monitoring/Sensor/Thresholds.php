<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Thresholds.
 */

namespace Drupal\monitoring\Sensor;

use Drupal\monitoring\Result\SensorResultInterface;

/**
 * Determine status based on thresholds during sensor run.
 *
 * @see \Drupal\monitoring\Result\SensorResult::assessThresholds()
 */
class Thresholds {

  /**
   * The SensorInfo instance.
   *
   * @var \Drupal\monitoring\Sensor\SensorInfo
   */
  protected $sensorInfo;

  /**
   * The message that will be added to the result status message.
   *
   * @var string
   */
  protected $message;

  /**
   * Constructs a Thresholds object.
   *
   * @param \Drupal\monitoring\Sensor\SensorInfo $sensor_info
   *   The SensorInfo instance.
   */
  function __construct(SensorInfo $sensor_info) {
    $this->sensorInfo = $sensor_info;
  }

  /**
   * Gets status based on given value.
   *
   * Note that if the threshold value is NULL or an empty string no assessment
   * will be carried out therefore the OK value will be returned.
   *
   * @param int $value
   *   The sensor value to assess.
   *
   * @return int
   *   The assessed sensor status.
   */
  public function getMatchedThreshold($value) {
    if (method_exists($this, $this->sensorInfo->getThresholdsType())) {
      $status = $this->{$this->sensorInfo->getThresholdsType()}($value);
      if ($status !== NULL) {
        return $status;
      }
      return SensorResultInterface::STATUS_OK;
    }
    else {
      $this->message = format_string('Unknown threshold type @type', array('@type' => $this->$this->sensorInfo->getThresholdsType()));
      return SensorResultInterface::STATUS_CRITICAL;
    }
  }

  /**
   * Gets status message based on the status and threshold type.
   *
   * @return string
   *   Status message
   */
  public function getStatusMessage() {
    return $this->message;
  }

  /**
   * Checks if provided value exceeds the configured threshold.
   *
   * @param int $value
   *   The value to check.
   *
   * @return int|null
   *   A sensor status or NULL.
   */
  protected function exceeds($value) {
    if (($threshold = $this->sensorInfo->getThresholdValue('critical')) && $value > $threshold) {
      $this->message = format_string('exceeds @expected', array('@expected' => $threshold));
      return SensorResultInterface::STATUS_CRITICAL;
    }
    if (($threshold = $this->sensorInfo->getThresholdValue('warning')) && $value > $threshold) {
      $this->message = format_string('exceeds @expected', array('@expected' => $threshold));
      return SensorResultInterface::STATUS_WARNING;
    }
  }

  /**
   * Checks if provided value falls below the configured threshold.
   *
   * @param int $value
   *   The value to check.
   *
   * @return int|null
   *   A sensor status or NULL.
   */
  protected function falls($value) {
    if (($threshold = $this->sensorInfo->getThresholdValue('critical')) && $value < $threshold) {
      $this->message = format_string('falls below @expected', array('@expected' => $threshold));
      return SensorResultInterface::STATUS_CRITICAL;
    }
    if (($threshold = $this->sensorInfo->getThresholdValue('warning')) && $value < $threshold) {
      $this->message = format_string('falls below @expected', array('@expected' => $threshold));
      return SensorResultInterface::STATUS_WARNING;
    }
  }

  /**
   * Checks if provided value falls inside the configured interval.
   *
   * @param int $value
   *   The value to check.
   *
   * @return int|null
   *   A sensor status or NULL.
   */
  protected function inner_interval($value) {
    if (($low = $this->sensorInfo->getThresholdValue('critical_low')) && ($high = $this->sensorInfo->getThresholdValue('critical_high'))) {
      if ($value > $low && $value < $high) {
        $this->message = format_string('violating the interval @low - @high', array('@low' => $low, '@high' => $high));
        return SensorResultInterface::STATUS_CRITICAL;
      }
    }
    if (($low = $this->sensorInfo->getThresholdValue('warning_low')) && ($high = $this->sensorInfo->getThresholdValue('warning_high'))) {
      if ($value > $low && $value < $high) {
        $this->message = format_string('violating the interval @low - @high', array('@low' => $low, '@high' => $high));
        return SensorResultInterface::STATUS_WARNING;
      }
    }
  }

  /**
   * Checks if provided value is outside of the configured interval.
   *
   * @param int $value
   *   The value to check.
   *
   * @return int|null
   *   A sensor status or NULL.
   */
  protected function outer_interval($value) {
    if (($low = $this->sensorInfo->getThresholdValue('critical_low')) && ($high = $this->sensorInfo->getThresholdValue('critical_high'))) {
      if ($value < $low || $value > $high) {
        $this->message = format_string('outside the allowed interval @low - @high', array('@low' => $low, '@high' => $high));
        return SensorResultInterface::STATUS_CRITICAL;
      }
    }
    if (($low = $this->sensorInfo->getThresholdValue('warning_low')) && ($high = $this->sensorInfo->getThresholdValue('warning_high'))) {
      if ($value < $low || $value > $high) {
        $this->message = format_string('outside the allowed interval @low - @high', array('@low' => $low, '@high' => $high));
        return SensorResultInterface::STATUS_WARNING;
      }
    }
  }

}
