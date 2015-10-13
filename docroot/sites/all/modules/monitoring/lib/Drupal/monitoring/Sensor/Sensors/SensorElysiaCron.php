<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorElysiaCron.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorThresholds;

/**
 * Monitors elysia cron channels for last execution.
 */
class SensorElysiaCron extends SensorThresholds {

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    // The channel name.
    $name = $this->info->getSetting('name');
    $query = db_select('elysia_cron', 'e')->fields('e', array($this->info->getSetting('metric')));
    $query->condition('name', $name);

    $value = $query->execute()->fetchField();

    // In case we are querying for last_run, the value is the seconds ago.
    if ($this->info->getSetting('metric') == 'last_run') {
      $value = REQUEST_TIME - $value;
      $result->addStatusMessage('@time ago', array('@time' => format_interval($value)));
    }
    else {
      // metric last_execution_time
      $result->addStatusMessage('at @time', array('@time' => format_date($value)));
    }

    $result->setValue($value);
  }
}
