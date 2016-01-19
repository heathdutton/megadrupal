<?php
/**
 * @file
 * Contains \Drupal\monitoring\Controller\SensorResultMetadataController.
 */

namespace Drupal\monitoring\Controller;

/**
 * Metadata controller class for the monitoring_sensor_result entity.
 */
class SensorResultMetadataController extends \EntityDefaultMetadataController {

  /**
   * {@inheritdoc}
   */
  public function entityPropertyInfo() {
    // Loading property information and make em better usable in here.
    $info = parent::entityPropertyInfo();
    $prop = &$info[$this->type]['properties'];

    // The timestamp should be rendered/shown as a date.
    $prop['timestamp']['type'] = 'date';

    return $info;
  }
}
