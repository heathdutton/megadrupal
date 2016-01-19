<?php

/**
 * @file
 * Contains plugin declaration and callbacks.
 *
 */

/**
 * Tamper plugin declaration.
 */
$plugin = array(
  'name' => t('Debug'),
  'form' => 'feeds_debug_tamper_form',
  'callback' => 'feeds_debug_tamper_callback',
  'multi' => 'direct',
  'category' => 'Debug',
);

/**
 * Settings form for the debug tamper.
 */
function feeds_debug_tamper_form($importer, $element_key, $settings) {

  $form = array();

  $form['watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('watchdog'),
    '#description' => t('Write a message into the Drupal watchdog, accessible under "admin/reports/dblog"'),
    '#default_value' => (isset($settings['watchdog']) ? $settings['watchdog'] : 1),
  );

  $form['errorlog'] = array(
    '#type' => 'checkbox',
    '#title' => t('error log'),
    '#description' => t('Write a message into the webserver\'s error log, usually found at "/var/log/apache/error.log"'),
    '#default_value' => (isset($settings['errorlog']) ? $settings['errorlog'] : 1),
  );

  $form['message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Drupal message'),
    '#description' => t('Display a simple Drupal message on screen'),
    '#default_value' => (isset($settings['message']) ? $settings['message'] : 0)
  );

  $form['dpm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Devel\'s dpm() message'),
    '#description' => t('Write a dpm() message on screen. Requires the !link module to be installed.', array('!link' => l('devel', 'https://www.drupal.org/project/devel'))),
    '#default_value' => (isset($settings['dpm']) ? $settings['dpm'] : (module_exists('devel'))),
    '#disabled' => !(module_exists('devel'))
  );

  return $form;
}

/**
 * Tamper callback for the debug tamper.
 */
function feeds_debug_tamper_callback($result, $item_key, $element_key, &$field, $settings) {

  if ($settings['watchdog']) {
    $variables = array(
      '%field' => var_export($field, TRUE),
      '%line' => var_export($result->items[$item_key], TRUE),
    );
    // Simple version.
    // watchdog('debug_tamper', 'field: %field, at line: %line', $variables, WATCHDOG_DEBUG);
    // Formatted version.
    watchdog('debug_tamper', '<strong> field: </strong> %field <br><strong> at line: </strong> %line', $variables, WATCHDOG_DEBUG);
  }

  if ($settings['errorlog']) {
    $variables = array(
      '!field' => serialize($field),
      '!line' => serialize($result->items[$item_key]),
    );
    error_log(t('debug_tamper: field: !field, at line: !line', $variables));
  }

  if ($settings['message']) {
    $variables = array(
      '!field' => var_export($field, TRUE),
      '!line' => var_export($result->items[$item_key], TRUE),
    );
    drupal_set_message(t('Debug tamper: <br><strong> field: </strong> !field<br><strong> at line: </strong> !line', $variables));
  }

  if ($settings['dpm'] && module_exists('devel')) {
    dpm($field, 'Debug tamper: field');
    dpm($result->items[$item_key], 'Debug tamper: at line');
  }
}