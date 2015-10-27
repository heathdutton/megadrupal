<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorSearchApi.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorThresholds;

/**
 * Monitors unindexed items for a search api index.
 *
 * Every instance represents a single index.
 *
 * Once all items are processed, the value should be 0.
 *
 * @see search_api_index_status()
 */
class SensorSearchApi extends SensorThresholds {

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    $indexes = search_api_index_load_multiple(array($this->info->getSetting('index_id')));
    $index = reset($indexes);

    $status = search_api_index_status($index);

    // Set amount of unindexed items.
    $result->setValue($status['total'] - $status['indexed']);
  }

}
