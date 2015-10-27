<?php
/**
 * @file
 * Admin page/form callbacks.
 */

use Drupal\monitoring\Sensor\DisabledSensorException;
use Drupal\monitoring\SensorRunner;
use Drupal\monitoring\Sensor\SensorConfigurableInterface;
use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorInfo;

/**
 * Sensors overview settings form.
 */
function monitoring_config_sensors_overview_form($form, &$form_state) {
  $options = array();
  $default_value = array();

  foreach (monitoring_sensor_info() as $sensor_name => $sensor_info) {
    $row = array(
      'category' => $sensor_info->getCategory(),
      'label' => $sensor_info->getLabel(),
      'description' => $sensor_info->getDescription(),
    );

    $row['status'] = $sensor_info->isEnabled() ? t('Enabled') : t('Disabled');

    $links = array();
    if ($sensor_info->isConfigurable()) {
      $links[] = array('title' => t('Settings'), 'href' => 'admin/config/system/monitoring/sensors/' . $sensor_name,
        'query' => array('destination' => 'admin/config/system/monitoring/sensors'));
    }

    $row['actions'] = '';
    if (!empty($links)) {
      $row['actions'] = theme('links', array('links' => $links));
    }

    $options[$sensor_name] = $row;
    $default_value[$sensor_name] = $sensor_info->isEnabled();
  }

  $header = array(
    'category' => t('Category'),
    'label' => t('Label'),
    'description' => t('Description'),
    'status' => t('Status'),
    'actions' => t('Actions'),
  );

  $form['sensors'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $default_value,
    '#attributes' => array(
      'id' => 'monitoring-sensors-config-overview',
    ),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Sensors overview settings form submit callback.
 */
function monitoring_config_sensors_overview_form_submit($form, &$form_state) {
  foreach ($form_state['values']['sensors'] as $sensor_name => $enabled) {
    if ($enabled) {
      monitoring_sensor_manager()->enableSensor($sensor_name);
    }
    else {
      monitoring_sensor_manager()->disableSensor($sensor_name);
    }
  }
}

/**
 * Page callback to output list of sensors.
 *
 * @return array
 *   Renderable array.
 */
function monitoring_reports_sensors_overview() {
  $rows = array();
  $runner = new SensorRunner();
  $results = $runner->runSensors();

  $status_overview = array(
    SensorResultInterface::STATUS_OK => 0,
    SensorResultInterface::STATUS_INFO => 0,
    SensorResultInterface::STATUS_WARNING => 0,
    SensorResultInterface::STATUS_CRITICAL => 0,
    SensorResultInterface::STATUS_UNKNOWN => 0,
  );
  $total_execution_time = 0;
  $non_cached_execution_time = 0;
  // Oldest sensor age in seconds.
  $oldest_sensor_age = 0;
  // Oldest sensor info.
  $oldest_sensor_info = NULL;

  // Include views admin styles for the dropbutton.
  module_load_include('inc', 'views', 'includes/admin');
  views_ui_add_admin_css();

  foreach (monitoring_sensor_info_by_categories() as $category => $category_sensor_info) {

    // Category grouping row.
    $rows[] = array(
      'data' => array(
        'label' => array(
          'data' => '<h3>' . $category . '</h3>',
          'colspan' => 7
        ),
      ),
    );
    $ok_row_count = 0;

    /** @var \Drupal\monitoring\Sensor\SensorExtendedInfoInterface $sensor_info */
    foreach ($category_sensor_info as $sensor_name => $sensor_info) {
      if (!isset($results[$sensor_name])) {
        continue;
      }

      $sensor_result = $results[$sensor_name];
      $called_before = REQUEST_TIME - $sensor_result->getTimestamp();
      if ($called_before > $oldest_sensor_age) {
        $oldest_sensor_info = $sensor_info;
        $oldest_sensor_age = $called_before;
      }

      $row['data']['label'] = '<span title="' . $sensor_info->getDescription() . '">' . $sensor_info->getLabel() . '</span>';
      $row['data']['sensor_status'] = array(
        'data' => $sensor_result->getStatusLabel(),
        'class' => array('status', 'having-dropbutton'),
      );

      $row['data']['timestamp'] = format_interval($called_before);
      $row['data']['execution_time'] = array(
        'data' => $sensor_result->getExecutionTime() . 'ms',
        'class' => array('execution-time'),
      );
      $row['data']['sensor_status_message'] = truncate_utf8(strip_tags($sensor_result->getMessage()), 200, TRUE, TRUE);

      $row['class'] = array('monitoring-sensor-' . $sensor_name, 'monitoring-' . strtolower($sensor_result->getStatus()));

      $links = array();
      $links['details'] = array('title' => t('Details'), 'href' => 'admin/reports/monitoring/sensors/' . $sensor_name);
      // Display a force execution link for any sensor that can be cached.
      if ($sensor_info->getCachingTime() && user_access('monitoring force run')) {
        $links['force_execution'] = array('title' => t('Force execution'), 'href' => 'monitoring/sensors/force/' . $sensor_name);
      }
      if ($sensor_info->isConfigurable() && user_access('administer monitoring')) {
        $links['settings'] = array('title' => t('Settings'), 'href' => 'admin/config/system/monitoring/sensors/' . $sensor_name,
          'query' => array('destination' => 'admin/reports/monitoring'));
      }

      drupal_alter('monitoring_sensor_links', $links, $sensor_info);

      $row['data']['actions'] = array();
      if (!empty($links)) {
        $row['data']['actions']['data'] = theme('links__ctools_dropbutton',
          array('links' => $links));
      }

      $rows[] = $row;

      $status_overview[$sensor_result->getStatus()]++;
      $total_execution_time += $sensor_result->getExecutionTime();
      if (!$sensor_result->isCached()) {
        $non_cached_execution_time += $sensor_result->getExecutionTime();
      }
      if ($sensor_result->getStatus() == SensorResultInterface::STATUS_OK) {
        $ok_row_count++;
      }
      else {
        $ok_row_count = -1;
      }
    }

    // Add special css class if all sensors of a category are ok.
    if ($ok_row_count >= 0) {
      $index = count($rows) - $ok_row_count - 1;
      $rows[$index]['class'][] = 'sensor-category-ok';
    }
  }

  $output['summary'] = array(
    '#theme' => 'monitoring_overview_summary',
    '#status_overview' => $status_overview,
    '#total_execution_time' => $total_execution_time,
    '#non_cached_execution_time' => $non_cached_execution_time,
  );

  // We can add the oldest_sensor_* data only if there are sensor results cached.
  if (!empty($oldest_sensor_info)) {
    $output['summary']['#oldest_sensor_label'] = $oldest_sensor_info->getLabel();
    $output['summary']['#oldest_sensor_category'] = $oldest_sensor_info->getCategory();
    $output['summary']['#oldest_sensor_called_before'] = format_interval($oldest_sensor_age);
  }

  $header = array(
    t('Sensor name'),
    array('data' => t('Status'), 'class' => array('status')),
    t('Called before'),
    t('Execution time'),
    t('Status Message'),
    array('data' => t('Actions'), 'class' => array('actions')),
  );

  $monitoring_error_sensors = $status_overview[SensorResultInterface::STATUS_WARNING] +
      $status_overview[SensorResultInterface::STATUS_CRITICAL] +
      $status_overview[SensorResultInterface::STATUS_UNKNOWN];

  $output['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#sticky' => TRUE,
    '#attributes' => array(
      'class' => array('monitoring-severity-colors'),
      'id' => 'monitoring-sensors-overview',
    ),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'monitoring') . '/monitoring.css',
      ),
      'js' => array(
        array(
          'data' => drupal_get_path('module', 'monitoring') . '/monitoring.js',
          'type' => 'file',
        ),
        array(
          'data' => array('monitoring_error_sensors' => $monitoring_error_sensors),
          'type' => 'setting',
        ),
      ),
    ),
  );

  return $output;
}

/**
 * Service sensors setting form.
 */
function monitoring_sensor_settings_form($form, &$form_state, SensorInfo $sensor_info) {
  if (!$sensor_info->isConfigurable()) {
    return $form;
  }

  $sensor_class = $sensor_info->getSensorClass();
  /** @var SensorConfigurableInterface $sensor */
  $sensor = new $sensor_class($sensor_info);
  // Note that here we cannot set the sensor object as some sensor objects may
  // carry with itself PDOStatements that cannot be serialised.
  $form_state['sensor_name'] = $sensor_info->getName();

  $form_key = monitoring_sensor_settings_key($sensor_info->getName());

  $form[$form_key] = array(
    '#tree' => TRUE,
  );

  $form[$form_key] = $sensor->settingsForm($form[$form_key], $form_state);

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate sensor settings form.
 */
function monitoring_sensor_settings_form_validate($form, &$form_state) {
  $sensor_info = monitoring_sensor_manager()->getSensorInfoByName($form_state['sensor_name']);
  $sensor_class = $sensor_info->getSensorClass();
  /** @var SensorConfigurableInterface $sensor */
  // @todo move this to a factory.
  $sensor = new $sensor_class($sensor_info);
  $sensor->settingsFormValidate($form, $form_state);
}

/**
 * Submit sensor settings form.
 */
function monitoring_sensor_settings_form_submit($form, &$form_state) {
  $form_key = monitoring_sensor_settings_key($form_state['sensor_name']);
  monitoring_sensor_settings_save($form_state['sensor_name'], $form_state['values'][$form_key]);
  drupal_set_message(t('Sensor settings saved.'));
}

/**
 * Services sensor details form.
 */
function monitoring_sensor_details($form, &$form_state, SensorInfo $sensor_info) {
  $form_state['sensor_name'] = $sensor_info->getName();

  try {
    $result = monitoring_sensor_run($sensor_info->getName(), FALSE, TRUE);
  }
  catch (DisabledSensorException $e) {
    // Exception is thrown if the sensor is not enabled.
    // Display a page not found in that case.
    drupal_not_found();
    drupal_exit();
  }

  if ($sensor_info->getDescription()) {
    $form['sensor_info']['description'] = array(
      '#type' => 'item',
      '#title' => t('Description'),
      '#markup' => $sensor_info->getDescription(),
    );
  }

  if ($sensor_info->getCategory()) {
    $form['sensor_info']['category'] = array(
      '#type' => 'item',
      '#title' => t('Category'),
      '#markup' => $sensor_info->getCategory(),
    );
  }

  $form['sensor_result'] = array(
    '#type' => 'fieldset',
    '#title' => t('Result'),
  );

  $form['sensor_result']['status'] = array(
    '#type' => 'item',
    '#title' => t('Status'),
    '#markup' => $result->getStatusLabel(),
  );

  $form['sensor_result']['message'] = array(
    '#type' => 'item',
    '#title' => t('Message'),
    '#markup' => $result->getMessage(),
  );


  $form['sensor_result']['execution_time'] = array(
    '#type' => 'item',
    '#title' => t('Execution time'),
    '#markup' => $result->getExecutionTime() . 'ms',
  );

  if ($result->isCached()) {
    $form['sensor_result']['cached'] = array(
      '#type' => 'item',
      '#title' => t('Cache information'),
      '#markup' => t('Executed @interval ago, valid for @valid', array('@interval' => format_interval(REQUEST_TIME - $result->getTimestamp()), '@valid' => format_interval($sensor_info->getCachingTime()))),
    );

    $form['sensor_result']['force_run'] = array(
      '#type' => 'submit',
      '#value' => t('Run now'),
      '#access' => user_access('monitoring force run'),
    );
  }
  elseif ($sensor_info->getCachingTime()) {
    $form['sensor_result']['cached'] = array(
      '#type' => 'item',
      '#title' => t('Cache information'),
      '#markup' => t('Executed now, valid for @valid', array('@valid' => format_interval($sensor_info->getCachingTime()))),
    );

    $form['sensor_result']['force_run'] = array(
      '#type' => 'submit',
      '#value' => t('Run again'),
      '#access' => user_access('monitoring force run'),
    );
  }
  else {
    $form['sensor_result']['cached'] = array(
      '#type' => 'item',
      '#title' => t('Cache information'),
      '#markup' => t('Not cached'),
    );
  }

  if ($sensor_info->isExtendedInfo()) {
    $form['sensor_result']['verbose'] = array(
      '#type' => 'fieldset',
      '#title' => t('Verbose'),
      '#access' => user_access('monitoring verbose'),
    );
    if ($result->isCached()) {
      $form['sensor_result']['verbose']['output'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('Verbose output is not available for cached sensor results. Click force run to see verbose output.') . '</p>',
      );
    }
    elseif ($verbose_output = $result->getVerboseOutput()) {
      if (is_array($verbose_output)) {
        $form['sensor_result']['verbose']['output'] = $verbose_output;
      }
      else {
        $form['sensor_result']['verbose']['output'] = array(
          '#type' => 'markup',
          '#markup' => '<pre>' . $verbose_output . '</pre>',
        );
      }
    }
    else {
      $form['sensor_result']['verbose']['output'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('No verbose output available for this sensor execution.') . '</p>',
      );
    }
  }

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#description' => '<pre>' . var_export($sensor_info->getSettings(), TRUE) . '</pre>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $view = views_get_view('monitoring_sensor_results');
  if (!empty($view)) {
    $view->init_display();
    $output = $view->preview('detail_page_log', array($sensor_info->getName()));
    if (!empty($view->result)) {
      $form['sensor_log'] = array(
        '#type' => 'fieldset',
        '#title' => t('Log'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['sensor_log']['view']['#markup'] = $output;
    }
  }

  return $form;
}

/**
 * Submit sensor details form.
 */
function monitoring_sensor_details_submit($form, &$form_state) {
  SensorRunner::resetCache(array($form_state['sensor_name']));
  drupal_set_message(t('Sensor force run executed.'));
}

/**
 * Page callback to clear sensor caches.
 */
function monitoring_force_execution($sensor_name = NULL) {
  if ($sensor_name) {
    // If a sensor was passed, reset just that.
    SensorRunner::resetCache(array($sensor_name));
  }
  else {
    // Otherwise reset all.
    SensorRunner::resetCache();
  }
  drupal_goto('admin/reports/monitoring');
}

/**
 * Monitoring settings form.
 *
 * Configure logging.
 */
function monitoring_settings_form($form, &$form_state) {
  $form['monitoring_sensor_call_logging'] = array(
    '#type' => 'select',
    '#title' => t('Monitoring event logging'),
    '#description' => t('Control local logging of sensor call results.'),
    '#options' => array(
      'all' => t('Log all events'),
      'on_request' => t('Log only on request or on status change'),
      'none' => t('No logging')
    ),
    '#default_value' => variable_get('monitoring_sensor_call_logging', 'on_request'),
  );

  return system_settings_form($form);
}

/**
 * Ajax callback for threshold settings.
 */
function monitoring_sensor_thresholds_ajax($form, &$form_state) {
  $sensor_name = $form_state['sensor_name'];
  $form_key = monitoring_sensor_settings_key($sensor_name);
  return $form[$form_key]['thresholds'];
}
