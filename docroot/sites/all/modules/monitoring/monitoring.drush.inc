<?php
/**
 * @file
 * Drush support for monitoring.
 */

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\DisabledSensorException;
use Drupal\monitoring\Sensor\NonExistingSensorException;

/**
 * Exit code for a critical sensor status.
 */
define('MONITORING_DRUSH_SENSOR_STATUS_CRITICAL', 3);
/**
 * Exit code for an unknown sensor status.
 */
define('MONITORING_DRUSH_SENSOR_STATUS_UNKNOWN', 2);
/**
 * Exit code for a warning sensor status.
 */
define('MONITORING_DRUSH_SENSOR_STATUS_WARNING', 1);
/**
 * Exit code for a proper run with OK status.
 */
define('MONITORING_DRUSH_SENSOR_STATUS_OK', 0);

/**
 * Implements hook_drush_command().
 *
 * Provides commands:
 * - monitoring-info
 * - monitoring-run
 * - monitoring-enable
 * - monitoring-disable
 * - monitoring-purge-settings
 */
function monitoring_drush_command() {
  $items = array();

  $items['monitoring-info'] = array(
    'callback' => 'monitoring_drush_info',
    'description' => 'Displays information about available sensors. If a sensor name provided as argument a detailed sensor info is provided.',
    'arguments' => array(
      'sensor_name' => 'Specific sensor name for which we want to display info.'
    ),
    'examples' => array(
      'drush monitoring-info' => 'Prints sensor info for all available sensors.',
      'drush monitoring-info node_new' => 'Prints info of the node_new sensor.',
    ),
    'drupal dependencies' => array('monitoring'),
  );

  $items['monitoring-run'] = array(
    'callback' => 'monitoring_drush_run',
    'description' => 'Runs specific sensor and provides verbose data.',
    'arguments' => array(
      'sensor_name' => 'Sensor name to invoke.',
    ),
    'options' => array(
      'verbose' => 'Display verbose information.',
      'force' => 'If the sensor execution should be forced in case cached result is available.',
      'output' => 'The output format. Currently "table" and "json" available. Defaults to "table".',
      'expand' => 'Relevant only for the json output. Currently "sensor_info" value supported.',
      'show-exec-time' => 'Relevant for the table output listing all results. Will expand the table with execution time info.'
    ),
    'examples' => array(
      'drush monitoring-result monitoring_git_dirty_tree' => 'Runs sensor to monitor the git status.',
      'drush monitoring-result --verbose monitoring_git_dirty_tree' => 'Runs sensor to monitor the git status and displays verbose information.',
      'drush monitoring-result --output=json --watchdog=disable' => 'Will output the sensor results in json format. The option --watchdog=disable will suppress watchdog output to console.',
    ),
    'drupal dependencies' => array('monitoring'),
  );

  $items['monitoring-enable'] = array(
    'callback' => 'monitoring_drush_enable',
    'description' => 'Enable specified monitoring sensor.',
    'arguments' => array(
      'sensor_name' => 'Sensor name to enable.',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush monitoring-enable monitoring_git_dirty_tree' => 'Enables monitoring_git_dirty_tree sensor.',
    ),
    'drupal dependencies' => array('monitoring'),
  );

  $items['monitoring-disable'] = array(
    'callback' => 'monitoring_drush_disable',
    'description' => 'Disable specified monitoring sensor.',
    'arguments' => array(
      'sensor_name' => 'Sensor name to disable.',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush monitoring-disable monitoring_git_dirty_tree' => 'Disables monitoring_git_dirty_tree sensor.',
    ),
    'drupal dependencies' => array('monitoring'),
  );

  $items['monitoring-purge-settings'] = array(
    'callback' => 'monitoring_drush_purge_settings',
    'description' => 'Purges settings for a specific sensor, or all sensors.',
    'arguments' => array(
      'sensor_name' => 'Sensor name for which to purge settings.',
    ),
    'examples' => array(
      'drush monitoring-purge-settings monitoring_git_dirty_tree' => 'Enables monitoring_git_dirty_tree sensor.',
    ),
    'drupal dependencies' => array('monitoring'),
  );

  return $items;
}

/**
 * Drush callback to get available sensors info.
 *
 * Shows either all sensors as a table or a specific one.
 *
 * @param string $sensor_name
 *   Sensor name for which to print info.
 */
function monitoring_drush_info($sensor_name = NULL) {
  if (empty($sensor_name)) {
    monitoring_drush_info_all();
  }
  else {
    monitoring_drush_info_single($sensor_name);
  }
}

/**
 * Provides info about available sensors.
 */
function monitoring_drush_info_all() {
  $sensors_info = monitoring_sensor_manager()->getSensorInfo();

  $rows[] = array(dt('Label'), dt('Name'), dt('Category'), dt('Enabled'));
  $rows[] = array('----', '----', '----');
  /** @var \Drupal\monitoring\Sensor\SensorInfo $info */
  foreach ($sensors_info as $name => $info) {
    $rows[] = array(
      $info->getLabel(),
      $name,
      $info->getCategory(),
      ($info->isEnabled() ? t('Yes') : t('No')),
    );
  }

  drush_print_table($rows, TRUE);
}

/**
 * Prints detailed info about the given sensor $sensor_name.
 *
 * @param string $sensor_name
 *   Sensor name for which to print info.
 */
function monitoring_drush_info_single($sensor_name) {
  $sensor_info = NULL;
  try {
    $sensor_info = monitoring_sensor_manager()->getSensorInfoByName($sensor_name);
  }
  catch (NonExistingSensorException $e) {
    return drush_set_error('MONITORING_SENSOR_INVALID_NAME', dt('Sensor "@name" does not exist.', array('@name' => $sensor_name)));
  }

  $rows[] = array(format_string("@label (@name)", array('@label' => $sensor_info->getLabel(), '@name' => $sensor_info->getName())), '====================');
  $rows[] = array(dt('Category'), $sensor_info->getCategory());
  $rows[] = array(dt('Description'), $sensor_info->getDescription());
  $rows[] = array(dt('Value info'), format_string('type: @type, label: @label, numeric: @numeric', array(
    '@type' => ($sensor_info->getValueType() ? $sensor_info->getValueType() : dt('N/A')),
    '@label' => ($sensor_info->getValueLabel() ? $sensor_info->getValueLabel() : dt('N/A')),
    '@numeric' => ($sensor_info->isNumeric() ? dt('Yes') : dt('No')),
  )));
  $rows[] = array(dt('Caching time'), format_interval($sensor_info->getCachingTime()));
  $rows[] = array(dt('Enabled'), $sensor_info->isEnabled() ? dt('Yes') : dt('No'));
  $rows[] = array(dt('Has thresholds'), $sensor_info->isDefiningThresholds() ? dt('Yes') : dt('No'));
  $rows[] = array(dt('Is configurable'), $sensor_info->isConfigurable() ? dt('Yes') : dt('No'));

  drush_print_table($rows);
}

/**
 * Drush callback to get sensor results.
 *
 * @param string $sensor_name
 *   Sensor name to run.
 *
 * @return int
 *   The most escalated sensor status.
 *   0 - OK
 *   1 - WARNING
 *   2 - UNKNOWN
 *   3 - CRITICAL
 */
function monitoring_drush_run($sensor_name = NULL) {
  $force_run = (bool) drush_get_option('force');
  $verbose = (bool) drush_get_option('verbose');
  $output = drush_get_option('output', 'table');
  $expand = drush_get_option('expand');
  $show_exec_time = drush_get_option('show-exec-time');

  try {
    $sensor_names = array();
    if (!empty($sensor_name)) {
      $sensor_names = array($sensor_name);
    }
    $results = monitoring_sensor_run_multiple($sensor_names, $force_run, $verbose);
  }
  catch (NonExistingSensorException $e) {
    drush_set_error('MONITORING_SENSOR_INVALID_NAME', dt('Sensor "@name" does not exist.', array('@name' => $sensor_name)));
    return MONITORING_DRUSH_SENSOR_STATUS_UNKNOWN;
  }
  catch (DisabledSensorException $e) {
    drush_set_error('MONITORING_SENSOR_DISABLED', dt('Sensor "@name" is not enabled.', array('@name' => $sensor_name)));
    return MONITORING_DRUSH_SENSOR_STATUS_UNKNOWN;
  }

  if ($output == 'table') {
    monitoring_drush_result_output_table($results, $show_exec_time);
  }
  elseif ($output == 'json') {
    monitoring_drush_result_output_json($results, $expand);
  }
  else {
    drush_set_error('MONITORING_UNKNOWN_OUTPUT', dt('Unknown output @output.', array('@output' => $output)));
    return MONITORING_DRUSH_SENSOR_STATUS_UNKNOWN;
  }

  $status = MONITORING_DRUSH_SENSOR_STATUS_OK;
  foreach ($results as $result) {
    if ($result->isCritical() && in_array($status, array(MONITORING_DRUSH_SENSOR_STATUS_UNKNOWN, MONITORING_DRUSH_SENSOR_STATUS_WARNING, MONITORING_DRUSH_SENSOR_STATUS_OK))) {
      $status = MONITORING_DRUSH_SENSOR_STATUS_CRITICAL;
    }
    elseif ($result->isUnknown() && in_array($status, array(MONITORING_DRUSH_SENSOR_STATUS_WARNING, MONITORING_DRUSH_SENSOR_STATUS_OK))) {
      $status = MONITORING_DRUSH_SENSOR_STATUS_UNKNOWN;
    }
    elseif ($result->isWarning() && $status == MONITORING_DRUSH_SENSOR_STATUS_OK) {
      $status = MONITORING_DRUSH_SENSOR_STATUS_WARNING;
    }
  }

  // Set the exit status based on the sensor status.
  return $status;
}

/**
 * Results output in JSON format.
 *
 * @param \Drupal\monitoring\Result\SensorResultInterface[] $results
 *   List of sensor result objects.
 * @param string $expand
 *   Additional data to be expanded.
 */
function monitoring_drush_result_output_json(array $results, $expand = NULL) {
  $json_output = array();
  foreach ($results as $result) {
    $json_output[$result->getSensorName()] = $result->toArray();
    if ($expand == 'sensor_info') {
      $json_output[$result->getSensorName()]['sensor_info'] = $result->getSensorInfo()->toArray();
    }
  }
  drush_print(drupal_json_encode($json_output));
}

/**
 * Outputs human readable table with results.
 *
 * @param \Drupal\monitoring\Result\SensorResultInterface[] $results
 *   List of sensor result objects.
 * @param bool $show_exec_time
 *   If TRUE the multi results view will display also execution time.
 */
function monitoring_drush_result_output_table(array $results, $show_exec_time = FALSE) {
  if (count($results) == 1) {
    monitoring_drush_result_output_table_single(reset($results));
  }
  else {
    monitoring_drush_result_output_table_multiple($results, $show_exec_time);
  }
}

/**
 * Outputs single sensor result.
 *
 * @param \Drupal\monitoring\Result\SensorResultInterface $result
 *   Sensor result object.
 */
function monitoring_drush_result_output_table_single(SensorResultInterface $result) {
  $rows[] = array(dt('Name'), $result->getSensorInfo()->getName());
  $rows[] = array(dt('Label'), $result->getSensorInfo()->getLabel());
  $rows[] = array(dt('Status'), $result->getStatusLabel());
  $rows[] = array(dt('Message'), $result->getMessage());
  $rows[] = array(dt('Execution time'), $result->getExecutionTime() . 'ms');
  $rows[] = array(dt('Result age'), format_interval(time() - $result->getTimestamp()));

  // Add the verbose output if requested.
  if ($result->getVerboseOutput()) {
    $rows[] = array(dt('Verbose output'), $result->getVerboseOutput());
  }

  drush_print_table($rows);
}

/**
 * Outputs sensor results table for multiple results.
 *
 * @param \Drupal\monitoring\Result\SensorResultInterface[] $results
 *   List of sensor result objects.
 * @param bool $show_exec_time
 *   If TRUE the multi results view will display also execution time.
 */
function monitoring_drush_result_output_table_multiple(array $results, $show_exec_time = FALSE) {
  $rows['header'] = array(dt('Sensor'), dt('Status'), dt('Message'), dt('Result age'));
  if ($show_exec_time) {
    $rows['header'][] = dt('Execution time');
  }

  foreach ($results as $result) {
    $rows[$result->getSensorName()] = array(
      format_string("@label\n(@name)", array('@label' => truncate_utf8($result->getSensorInfo()->getLabel(), 40, TRUE, TRUE), '@name' => $result->getSensorName())),
      $result->getStatusLabel(),
      truncate_utf8($result->getMessage(), 40, TRUE, TRUE),
      format_interval(time() - $result->getTimestamp()),
    );

    if ($show_exec_time) {
      $rows[$result->getSensorName()][] = $result->getExecutionTime() . 'ms';
    }
  }

  drush_print_table($rows);
}

/**
 * Drush callback to enable a sensor.
 *
 * @param string $sensor_name
 *   Sensor name.
 */
function monitoring_drush_enable($sensor_name) {
  $sensor_manager = monitoring_sensor_manager();
  try {
    $sensor_info = $sensor_manager->getSensorInfoByName($sensor_name);
    if (!$sensor_info->isEnabled()) {
      $sensor_manager->enableSensor($sensor_name);
      drush_log(dt('The sensor @name was enabled.', array('@name' => $sensor_info->getLabel())), 'ok');
    }
    else {
      drush_log(dt('The sensor @name is already enabled.', array('@name' => $sensor_info->getLabel())), 'warning');
    }

  }
  catch (NonExistingSensorException $e) {
    drush_set_error('MONITORING_SENSOR_INVALID_NAME', dt('Sensor "@name" does not exist.', array('@name' => $sensor_name)));
  }
}

/**
 * Drush callback to disable a sensor.
 *
 * @param string $sensor_name
 *   Sensor name.
 */
function monitoring_drush_disable($sensor_name) {
  $sensor_manager = monitoring_sensor_manager();
  try {
    $sensor_info = $sensor_manager->getSensorInfoByName($sensor_name);
    if ($sensor_info->isEnabled()) {
      $sensor_manager->disableSensor($sensor_name);
      drush_log(dt('The sensor @name was disabled.', array('@name' => $sensor_info->getLabel())), 'ok');
    }
    else {
      drush_log(dt('The sensor @name is already disabled.', array('@name' => $sensor_info->getLabel())), 'warning');
    }
  }
  catch (NonExistingSensorException $e) {
    drush_set_error('MONITORING_SENSOR_INVALID_NAME', dt('Sensor "@name" does not exist.', array('@name' => $sensor_name)));
  }
}

/**
 * Drush callback to purge sensor settings.
 *
 * @param string $sensor_name
 *   Sensor name for which to purge settings.
 */
function monitoring_drush_purge_settings($sensor_name = NULL) {
  if (empty($sensor_name)) {
    if (drush_confirm(dt('Do you want to purge all sensor settings?'))) {
      $query = db_query('DELETE FROM {variable} WHERE name LIKE :name', array(':name' => 'monitoring_%'));
      drush_print(dt('Purged settings of @count sensors.', array('@count' => $query->rowCount())));
    }
    return;
  }

  $sensor_manager = monitoring_sensor_manager();
  try {
    // Check if the given sensor exists.
    $sensor_manager->getSensorInfoByName($sensor_name);
    $query = db_query('DELETE FROM {variable} WHERE name = :name', array(':name' => monitoring_sensor_settings_key($sensor_name)));
    if ($query->rowCount()) {
      drush_print(dt('Purged settings of @sensor_name.', array('@sensor_name' => $sensor_name)));
    }
    else {
      drush_print(dt('No settings to purge.'));
    }
  }
  catch (NonExistingSensorException $e) {
    drush_set_error('MONITORING_SENSOR_INVALID_NAME', dt('Sensor "@name" does not exist.', array('@name' => $sensor_name)));
  }
}
