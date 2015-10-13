<?php
/**
 * @file
 * Monitoring remote API resource callbacks.
 */

use Drupal\monitoring\Sensor\NonExistingSensorException;

/**
 * Sensor info resource.
 *
 * @param string $sensor_name
 *   Specific sensor name.
 *
 * @return array
 *   Sensor info data as array.
 */
function monitoring_services_sensor_info($sensor_name) {
  try {
    $response = monitoring_sensor_manager()->getSensorInfoByName($sensor_name)->toArray();
    $response['uri'] = services_resource_uri(array('sensor-info', $sensor_name));
    return $response;
  }
  catch (NonExistingSensorException $e) {
    services_error($e->getMessage(), 404);
  }
}

/**
 * Sensor info list resource.
 *
 * @return array
 *   List of all sensors info data.
 */
function monitoring_services_sensors_info() {
  $response = array();
  foreach (monitoring_sensor_manager()->getSensorInfo() as $sensor_name => $sensor_info) {
    $response[$sensor_name] = $sensor_info->toArray();
    $response[$sensor_name]['uri'] = services_resource_uri(array('sensor-info', $sensor_name));
  }

  return $response;
}

/**
 * Sensor result resource.
 *
 * @param string $sensor_name
 *   Specific sensor name.
 * @param string $expand
 *   Sensor result data to expand.
 *
 * @return array
 *   Single sensor result data.
 */
function monitoring_services_sensor_result($sensor_name, $expand = NULL) {
  try {
    $result = monitoring_sensor_run($sensor_name);
    $response = $result->toArray();
    $response['uri'] = services_resource_uri(array('sensor-result', $sensor_name));
    if ($expand == 'sensor_info') {
      $response['sensor_info'] = $result->getSensorInfo()->toArray();
    }
    return $response;
  }
  catch (InvalidArgumentException $e) {
    return services_error($e->getMessage(), 404);
  }
}

/**
 * Sensor result resource.
 *
 * @param string $expand
 *   Sensor result data to expand.
 *
 *
 * @return array
 *   List of all sensors results data.
 */
function monitoring_services_sensors_results($expand = NULL) {
  $response = array();
  foreach (monitoring_sensor_run_multiple() as $sensor_name => $sensor_result) {
    $response[$sensor_name] = $sensor_result->toArray();
    $response[$sensor_name]['uri'] = services_resource_uri(array('sensor-result', $sensor_name));
    if ($expand == 'sensor_info') {
      $response[$sensor_name]['sensor_info'] = $sensor_result->getSensorInfo()->toArray();
    }
  }

  return $response;
}

