<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorQueue.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorThresholds;

/**
 * Monitors number of items for a given core queue.
 *
 * Every instance represents a single queue.
 * Once all queue items are processed, the value should be 0.
 *
 * @see \DrupalQueue
 */
class SensorQueue extends SensorThresholds {

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    /** @var \DrupalQueueInterface $queue */
    $queue = \DrupalQueue::get($this->info->getSetting('queue'));
    $result->setValue($queue->numberOfItems());
  }
}
