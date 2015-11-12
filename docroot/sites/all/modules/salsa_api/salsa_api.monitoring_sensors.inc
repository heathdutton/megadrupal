<?php
/**
 * @file
 * Defines a sensor that checks connectivity to the Salsa service.
 */

/**
 * Implements hook_monitoring_sensor_info().
 */
function salsa_api_monitoring_sensor_info() {
  $info['salsa_api_connection'] = array(
    'label' => 'Salsa API connection',
    'description' => 'Checks the connection to the Salsa service',
    'sensor_class' => 'Drupal\salsa_api\Sensor\Sensors\SalsaApiConnection',
    'value_label' => 'milliseconds',
    'settings' => array(
      'category' => 'Other',
      'caching_time' => 600,
      'thresholds' => array(
        'type' => 'exceeds',
        'warning' => 500,
        'critical' => 1000,
      ),
    ),
  );

  return $info;
}
