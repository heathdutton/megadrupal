<?php

/**
 * @file
 * Drush integration for monitoring_icinga.module.
 */

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\DisabledSensorException;
use Drupal\monitoring\Sensor\NonExistingSensorException;

/**
 * Implements hook_drush_command().
 */
function monitoring_icinga_drush_command() {
  $items = array();

  $items['monitoring-icinga'] = array(
    'callback' => 'monitoring_icinga_drush_callback',
    'options' => array(
      'sensor_name' => 'Sensor name to invoke.',
    ),
    'description' => 'Run munin plugins.',
    'examples' => array(
      'drush monitoring-icinga plugin_name' => 'Run munin plugin.',
    ),
    'drupal dependencies' => array('monitoring_icinga'),
  );

  return $items;
}

/**
 * Drush callback to get the sensor info.
 *
 * @param string $sensor_name
 *   Sensor name.
 */
function monitoring_icinga_drush_callback($sensor_name = NULL) {

  // If we are provided with sensor name we expect an active check.
  if (!empty($sensor_name)) {
    monitoring_icinga_active_check($sensor_name);
  }

  $results = monitoring_sensor_run_multiple();
  $output = '';

  foreach ($results as $result) {
    $message = iconv('utf-8', 'ASCII', truncate_utf8(strip_tags($result->getMessage()), 124, TRUE, TRUE));
    $service_description = monitoring_icinga_service_description($result->getSensorInfo());

    // Map INFO status to OK.
    $status = $result->getStatus();
    if ($status == SensorResultInterface::STATUS_INFO) {
      $status = SensorResultInterface::STATUS_OK;
    }

    // Make sure the output does not have any new lines.
    $output .= preg_replace('/\s+/', ' ', $service_description . '|' . $status . '|' . $message) . '~';

  }
  drush_print($output, 0, NULL, FALSE);
}

/**
 * Prints sensor status message and exits with sensor status code.
 *
 * This callback should be used for active icinga checks where icinga directly
 * invokes the sensor and expects printed output and exit code.
 *
 * @param string $sensor_name
 *   Sensor name to run.
 */
function monitoring_icinga_active_check($sensor_name) {
  $statuses = monitoring_icinga_status_codes();

  try {
    $result = monitoring_sensor_run($sensor_name);
  }
  catch (NonExistingSensorException $e) {
    drush_set_error('MONITORING_SENSOR_INVALID_NAME', dt('Sensor "@name" does not exist.', array('@name' => $sensor_name)));
    return $statuses[SensorResultInterface::STATUS_CRITICAL];
  }
  catch (DisabledSensorException $e) {
    drush_set_error('MONITORING_SENSOR_DISABLED', dt('Sensor "@name" is not enabled.', array('@name' => $sensor_name)));
    return $statuses[SensorResultInterface::STATUS_CRITICAL];
  }

  drush_print(iconv('utf-8', 'ASCII', $result->getMessage()));

  // Map INFO status to OK.
  $status = $result->getStatus();
  if ($status == SensorResultInterface::STATUS_INFO) {
    $status = SensorResultInterface::STATUS_OK;
  }

  return $statuses[$status];
}
