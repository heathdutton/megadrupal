<?php
/**
 * @file
 * Contains \Drupal\monitoring\Controller\SensorResultController.
 */

namespace Drupal\monitoring\Controller;

/**
 * Controller class for the monitoring_sensor_result entity.
 */
class SensorResultController extends \EntityAPIController {

  /**
   * {@inheritdoc}
   */
  public function create(array $values = array()) {
    $entity = parent::create($values);

    if (empty($entity->timestamp)) {
      $entity->timestamp = REQUEST_TIME;
    }

    return $entity;
  }

}
