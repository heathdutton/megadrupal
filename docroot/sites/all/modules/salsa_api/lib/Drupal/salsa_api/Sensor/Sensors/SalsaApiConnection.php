<?php
/**
 * @file
 * Contains \Drupal\salsa_api\Sensor\Sensors\SalsaApiConnection.
 */

namespace Drupal\salsa_api\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorThresholds;

/**
 * Monitors the connection to the Salsa service.
 *
 * Displays a warning if credential variables were not set.
 */
class SalsaApiConnection extends SensorThresholds {

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    // Check whether connecting is possible at all.
    if (!variable_get('salsa_api_username', FALSE)
      || !variable_get('salsa_api_password', FALSE)
      || !variable_get('salsa_api_url', FALSE)) {
      $result->setValue(0);
      $result->addStatusMessage('Salsa API is not configured.');
      $result->setStatus(SensorResultInterface::STATUS_WARNING);
      return;
    }

    try {
      // Connect and measure time it takes in order to use it as sensor value.
      $before_execution = microtime(TRUE);
      salsa_api()->connect();
      $after_execution = microtime(TRUE);
      $result->setValue((int) (($after_execution - $before_execution) * 1000));
      $result->addStatusMessage('Connection was established successfully.');
      $result->setStatus(SensorResultInterface::STATUS_OK);
    }
    catch (\Exception $e) {
      $result->setValue(0);
      $result->addStatusMessage('Exception while connecting to Salsa service: ' . $e->getMessage());
      $result->setStatus(SensorResultInterface::STATUS_CRITICAL);
    }
  }

}
