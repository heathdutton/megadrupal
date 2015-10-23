<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorSimplenewsPending.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorThresholds;

/**
 * Monitors pending items in the simplenews mail spool.
 *
 * Once all is processed, the value should be 0.
 *
 * @see simplenews_count_spool()
 */
class SensorSimplenewsPending extends SensorThresholds {

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    module_load_include('inc', 'simplenews', 'includes/simplenews.mail');
    $result->setValue(simplenews_count_spool(array('status' => SIMPLENEWS_SPOOL_PENDING)));
  }
}
