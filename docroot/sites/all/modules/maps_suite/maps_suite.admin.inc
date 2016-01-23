<?php

/**
 * @file
 * Administrative functions for MaPS Suite module.
 */

use Drupal\maps_suite\Log\LogInterface;

/**
 * Menu callback; administrative overview.
 */
function maps_suite_admin_overview() {
  $rows = array();

  foreach (module_invoke_all('maps_suite_admin_overview') as $header => $content) {
    $row = array();
    $row[] = array(
      'header' => TRUE,
      'data' => $header,
    );

    if (is_array($content)) {
      $row = array_merge($row, $content);
    }
    else {
      $row[] = array(
        'data' => $content,
        'colspan' => 3,
      );
    }

    $rows[] = $row;
  }

  drupal_alter('maps_suite_admin_overview', $rows);
  return theme('table', array('rows' => $rows));
}

/**
 * Form API; Form builder for MaPS Suite Log settings.
 */
function maps_suite_admin_log_form($form, &$form_state) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings'),
  );

  $form['settings']['maps_suite_logs_enabled'] = array(
    '#title' => t('Enable logs'),
    '#type' => 'checkbox',
    '#description' => t('If disabled, logs will only appears in watchdog'),
    '#default_value' => variable_get('maps_suite_logs_enabled', TRUE),
  );

  $form['settings']['maps_suite_log_path'] = array(
    '#title' => t('Path to log files'),
    '#type' => 'textfield',
    '#default_value' => variable_get('maps_suite_log_path', LogInterface::LOG_DIR),
    '#description' => t('Define the path to the log files relative to Drupal file directory.'),
  );

  $form['settings']['maps_suite_log_oberver_watchdog_partial'] = array(
    '#title' => t('Enable Watchdog for partial log'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('maps_suite_log_oberver_watchdog_partial', 0),
    '#description' => t('If enabled, a Watchdog entry is added not only for completed log, but also for partial log (ie. log that are created through several operations.).'),
  );

  return system_settings_form($form);
}
