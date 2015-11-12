<?php
/**
 * @file
 * Monitoring API documentation.
 */

use Drupal\monitoring\Result\SensorResultInterface;

/**
 * Provides info about available sensors.
 *
 * Implementations of this hook can replace default contrib integration.
 *
 * @return array
 *   Sensor definition.
 *
 * @see \Drupal\monitoring\Sensor\SensorManager::loadSensorInfo()
 */
function hook_monitoring_sensor_info() {
  $sensors = array();

  // The sensor key / name is the unique identifier of the sensor instance.
  $sensor['cron_run'] = array(
    // UI label of the sensor. Should always be displayed in combination with
    // the category and does not have to unnecessarily repeat the category.
    'label' => 'Last cron run',
    // UI description to better understand the sensor purpose.
    'description' => 'Monitors cron run',
    // Sensor class that will trigger checks.
    'sensor_class' => 'CronRunMonitoring',
    // (optional) Result class. Default value is SensorResult.
    'result_class' => 'SensorResult',
    // (optional) The value type and therefore its presentation on UI.
    // Supported types are
    //   - time_interval: A time interval in seconds.
    //   - bool: Boolean states.
    //   - commerce_currency: Currency for e-commerce transactions.
    // See also monitoring_value_types(). Empty by default.
    'value_type' => 'time_interval',
    // (optional) May define a value label that will be used in the UI. The
    // value label is empty by default.
    'value_label' => 'Druplicons',
    // (optional) Defines if the sensor value is numeric. Defaults to TRUE.
    'numeric' => FALSE,
    // Sensor instance specific settings.
    'settings' => array(
      // (optional) The sensor category. Defaults to "Other".
      'category' => 'Cron',
      // Flag if to log sensor activity.
      'result_logging' => FALSE,
      // (optional) Set this to FALSE to prevent the sensor from being
      // triggered. Defaults to TRUE.
      'enabled' => TRUE,
      // (optional) Time in seconds during which the sensor result should be
      // cached. Defaults to 0 (caching disabled).
      'caching_time' => 0,
      // (optional) Result time interval. This will be added to the default
      // message automatically. Empty by default.
      'time_interval_value' => 900,
      // (optional) Define sensor value thresholds, which allow to have
      // (configurable) intervals that set the sensor status to warning or
      // critical based on the value. All sensors that extend from
      // \Drupal\monitoring\Sensor\SensorThresholds support thresholds. This
      // definition is only necessary to provide explicit default thresholds.
      'thresholds' => array(
        // Threshold type. This defines which of the additional keys
        // are supported. Exceeds and falls use warning/critical, the interval
        // types use the _low/_high keys.
        //   - exceeds: Escalates if the value exceeds the configured limits,
        //     warning must be lower than critical.
        //   - falls: Escalates if the value falls below the configured limits,
        //     warning must be higher than critical.
        //   - inner_interval: Escalates if the value is within the configured
        //     intervals, warning must be outside of critical.
        //   - outer_interval: Escalates if the value is outside of the
        //     configured intervals, warning must be inside of critical.
        'type' => 'exceeds',
        'warning' => 5,
        'critical' => 10,
        'warning_high' => 5,
        'warning_low' => 7,
        'critical_high' => 9,
        'critical_low' => 3,
      ),
    ),
  );

  return $sensors;
}

/**
 * Allows to alter sensor links on the sensor overview page.
 *
 * @param array $links
 *   Links to be altered.
 * @param \Drupal\monitoring\Sensor\SensorInfo $sensor_info
 *   Sensor info object of a sensor for which links are being altered.
 *
 * @see monitoring_reports_sensors_overview()
 */
function hook_monitoring_sensor_links_alter(&$links, \Drupal\monitoring\Sensor\SensorInfo $sensor_info) {

}

/**
 * Allows to alter sensor info before creation of SensorInfo instance.
 *
 * @param array $info
 *   Sensor info to be altered.
 *
 * @see \Drupal\monitoring\Sensor\SensorManager::loadSensorInfo()
 */
function hook_monitoring_sensor_info_alter(&$info) {

}