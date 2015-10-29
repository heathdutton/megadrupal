<?php
// $Id: statspro_settings.inc,v 1.1.2.16 2010/11/05 23:02:14 rsevero Exp $

/**
 * @file
 *   General settings help methods.
 *
 */

if (module_exists('charts_graphs')) {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro_graphs.inc';
}

/**
 * Helper function to set initial values for session variables related to per
 * report period definitions.
 *
 * Session variable names get the provided suffix to differentiated them from
 * similar variables for other reports.
 *
 * @param <string> $suffix
 * @param <string> $period
 * @param <int> $number_of_days
 */
function set_default_session_values($suffix, $period = '', $number_of_days = NULL) {
  $index = 'statspro_period' . $suffix;
  if (array_key_exists($period, statspro_get_period_items())) {
    $_SESSION[$index] = $period;
  }
  elseif (!isset($_SESSION[$index])) {
    $_SESSION[$index] = variable_get('statspro_period', 'today');
  }

  $index = 'statspro_custom_number_days' . $suffix;
  if (is_numeric($number_of_days)) {
    $_SESSION[$index] = (int) $number_of_days;
  }
  elseif (!isset($_SESSION[$index])) {
    $_SESSION[$index] = variable_get('statspro_custom_number_days', 30);
  }
}

/**
 * Form generating function for report settings.
 */
function statspro_settings_form($default_period, $default_custom_days) {
  $form = array();

  $form['statspro_period'] = array(
    '#type' => 'select',
    '#title' => t('Time period'),
    '#default_value' => $default_period,
    '#options' => statspro_get_period_items(),
    '#required' => TRUE,
  );

  $form['statspro_custom_number_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom number of days'),
    '#default_value' => $default_custom_days,
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Number of past days from today to include.'),
  );

  $form['#validate'] = array('statspro_settings_form_validate');

  $form['buttons']['#weight'] = 50;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#theme'] = 'system_settings_form';

  drupal_add_js(drupal_get_path('module', 'statspro') . '/statspro_common_settings.js');

  return $form;
}

/**
 * Implements hook_modulename_validate() for settings form().
 */
function statspro_settings_form_validate($form, &$form_state) {
  $periods = statspro_get_period_items();
  if (!in_array($form_state['values']['statspro_period'], array_keys($periods))) {
    form_set_error(
      'statspro_period',
      t(
        'Unknown period %period.',
        array('%period' => $form_state['values']['statspro_period'])
      )
    );
  }
  elseif ($form_state['values']['statspro_period'] == 'custom_days') {
    if (!is_numeric($form_state['values']['statspro_custom_number_days'])) {
      form_set_error(
        'statspro_custom_number_days',
        t(
          'You must set the a numeric value for the number of days for period %period.',
          array('%period' => $periods['custom_days'])
        )
      );
    }
    elseif ($form_state['values']['statspro_custom_number_days'] < 0) {
      form_set_error(
        'statspro_custom_number_days',
        t(
          'You must set a non negative number of days for period %period.',
          array('%period' => $periods['custom_days'])
        )
      );
    }
  }
}

/**
 * Informs if the mandatory dependencies for path aggregate reports generation
 * are met.
 *
 * @return boolean
 */
function statspro_path_aggregated_mandatory() {
  $dependency_ok = TRUE;

  if (module_exists('statistics')) {
    if (variable_get('statistics_enable_access_log', 0) == 0) {
      // Statistics module enabled but access log disabled.
      $message = t(
"The core %statistics_module module is enabled but the %access_log_setting
  setting in the !access_log page isn't. You can't have path aggregated
  statistics without it. Please consider enabling it to enable path aggregated
  reports. Remember that you should not discard access logs for as long as you
  want path aggregated reports.",
          array(
        '%statistics_module' => t('Statistics'),
        '%access_log_setting' => t('Enable access log'),
        '!access_log' => l(t('Access log settings'), 'admin/config/system/statistics'),
      )
        );
      drupal_set_message($message, 'error');
      $dependency_ok = FALSE;
    }
  }
  else {
    // Statistics module disabled.
    $message = t(
"The core %statistics_module module is not enabled. You can't have path
  aggregated statistics without it. Please consider enabling it and the
  %access_log_setting setting in the %access_log page. Remember that you should
  not discard access logs for as long as you want path aggregated reports.",
        array(
      '%statistics_module' => t('Statistics'),
      '%access_log_setting' => t('Enable access log'),
      '%access_log' => t('Access log settings'),
    )
      );
    drupal_set_message($message, 'error');
    $dependency_ok = FALSE;
  }

  return $dependency_ok;
}

/**
 * Returns the presentable period name and returns a period definition as number
 * of days for custom_days period.
 *
 * @param <string> $period
 * @param <int> $number_of_days
 * @return <array>
 */
function statspro_get_period_info($period, $number_of_days) {
  $period_name = NULL;
  if ($period == 'custom_days') {
    $period = $number_of_days;
    $period_name = t('custom period of @days past days', array('@days' => $number_of_days));
  }
  else {
    $periods = statspro_get_period_items();
    if (array_key_exists($period, $periods)) {
      $period_name = $periods[$period];
    }
  }
  return array(
    'period' => $period,
    'period_name' => $period_name,
  );
}

/**
 * Informs if charting dependencies are met.
 *
 * @return <mixed> array of available charting options or FALSE
 */
function statspro_get_available_charts_graphs() {
  static $dependency_ok = NULL;

  if ($dependency_ok === NULL) {
    $dependency_ok = FALSE;
    if (module_exists('charts_graphs')) {
      $charting_libraries = charts_graphs_apis();
      if (count($charting_libraries) != 0) {
        $dependency_ok = $charting_libraries;
      }
    }
  }

  return $dependency_ok;
}

/**
 * Includes link to ADvanced Help pages if the ADvanced Help module is
 * available.
 */
function statspro_include_link_to_advanced_help_pages(&$form) {
  if (!module_exists('advanced_help')) {
    return;
  }
  $form['advanced_help_link_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 110,
  );
  $message = "\n<p>" . t(
      "Please see !help_page for extra info on using this
        module.",
      array('!help_page' => l('Statistics Pro help pages', 'admin/advanced_help/statspro'))
    ) . "</p>\n";
  $form['advanced_help_link_wrapper']['advanced_help_link'] = array(
    '#value' => $message,
  );
}
