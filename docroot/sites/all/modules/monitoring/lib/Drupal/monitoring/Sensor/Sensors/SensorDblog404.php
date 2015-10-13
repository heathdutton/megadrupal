<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorDblog404.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;

/**
 * Monitors 404 page errors from dblog.
 *
 * Displays URL with highest occurrence as message.
 */
class SensorDblog404 extends SensorDatabaseAggregator {

  /**
   * {@inheritdoc}
   */
  public function buildQuery() {
    $query = parent::buildQuery();
    $query->addField('watchdog', 'message');
    // The message is the requested 404 URL.
    $query->groupBy('message');
    $query->orderBy('records_count', 'DESC');
    $query->range(0, 1);
    return $query;
  }

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    parent::runSensor($result);
    $query_result = $this->fetchObject();
    if (!empty($query_result) && !empty($query_result->message)) {
      $result->addStatusMessage($query_result->message);
    }
  }
}
