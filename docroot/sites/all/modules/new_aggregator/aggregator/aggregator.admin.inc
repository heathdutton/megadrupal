<?php
// $Id: aggregator.admin.inc,v 1.5 2008/07/21 18:33:26 aronnovak Exp $

/**
 * @file
 *   Administrative form for aggregator.
 */

/**
 * Provides a content-type tab form.
 * Processors and parsers should do form_alter() to provide their settings here
 */
function aggregator_settings_form($form_state) {
  $type = str_replace('-', '_', arg(3));
  $form = array();
  $period = array('-1' => t('none'));
  $period += drupal_map_assoc(array(900, 1800, 3600, 7200, 10800, 21600, 32400, 43200, 64800, 86400, 172800, 259200, 604800, 1209600, 2419200), 'format_interval');
  $parsers = module_implements('aggregator_parse');
  foreach ($parsers as $k => $v) {
    $info = module_invoke($v, 'aggregator_parse', 'info');
    unset($parsers[$k]);
    $parsers[$v] = $info['title'];
  }
  $processors = module_implements('aggregator_process');
  foreach ($processors as $k => $v) {
    $info = module_invoke($v, 'aggregator_process', 'info');
    unset($processors[$k]);
    $processors[$v] = $info['title'];
  }
  $form['type'] = array('#type' => 'value', '#value' => $type);  // For other modules' form_alter().
  $form['aggregator_feed_' . $type] = array(
    '#type' => 'checkbox',
    '#title' => t('Is a feed content type'),
    '#description' => t('Check if you would like to use this content type for downloading feeds to your site.'),
    '#default_value' => variable_get('aggregator_feed_' . $type, FALSE),
    '#weight' => -15,
  );
  $form['aggregator_parser_' . $type] = array(
    '#type' => 'radios',
    '#title' => t('Parser'),
    '#description' => t('Parsers are responsible for retrieving and parsing feed data.'),
    '#options' => $parsers,
    '#default_value' => variable_get('aggregator_parser_' . $type, array_pop($parsers)),
  );
  $form['aggregator_processor_' . $type] = array(
    '#type' => 'checkboxes',
    '#title' => t('Processors'),
    '#description' => t('Processors act on parsed feed data. Pick the processors suitable for your task.'),
    '#options' => $processors,
    '#default_value' => variable_get('aggregator_processor_' . $type, array_slice($processors, 0, 1)),
  );
  $form['aggregator_deduper_' . $type] = array(
    '#type' => 'hidden',
    '#value' => variable_get('aggregator_deduper_' . $type, array_pop(variable_get('aggregator_processor_' . $type, array_slice($processors, 0, 1)))),
  );
  $form['aggregator_refresh_' . $type] = array(
    '#type' => 'select',
    '#title' => t('Update interval'),
    '#default_value' => variable_get('aggregator_refresh_' . $type, 3600),
    '#options' => $period,
    '#description' => t('The approximate length of time between feed updates. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
  );
  $form['modules'] = array();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Stores the values in the {variable} table.
 */
function aggregator_settings_form_submit($form, &$form_state) {
  system_settings_form_submit($form, $form_state);
}
