<?php
/**
 * @file
 * Contains \Drupal\monitoring\Views\Handler\Field\SensorName.
 */

namespace Drupal\monitoring\Views\Handler\Field;

use Drupal\monitoring\Entity\SensorResultEntity;
use Drupal\monitoring\Sensor\NonExistingSensorException;

/**
 * Views handler to output sensor label and name.
 */
class SensorName extends \views_handler_field_entity {

  /**
   * {@inheritdoc}
   */
  function render($values) {
    /**
     * @var SensorResultEntity $result
     */
    $result = $this->get_value($values);
    try {
      $sensor_info = monitoring_sensor_manager()->getSensorInfoByName($result->sensor_name);
      $label = $sensor_info->getLabel();
    }
    catch (NonExistingSensorException $e) {
      $label = t('Disappeared sensor @name', array('@name' => $result->sensor_name));
    }

    return l($label, 'admin/reports/monitoring/sensors/' . $result->sensor_name);
  }
}
