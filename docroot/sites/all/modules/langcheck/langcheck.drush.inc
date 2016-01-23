<?php

/**
 * @file
 * Drush integration related functions.
 */

/**
 * Implements hook_drush_command().
 */
function langcheck_drush_command() {
  $items = array();
  $items['langcheck'] = array(
    'description' => 'Check all installed modules for translation percentage.',
    'arguments' => array(
      'action' => "Either 'check' or 'result'.",
    ),
    'examples' => array(
      'drush langcheck check' => 'Calculate the percentages.',
      'drush langcheck result' => 'Display the calculated percentages.',
    ),
  );
  return $items;
}

/**
 * Implements of drush_hook_COMMAND_validate().
 *
 * @param $action
 *   Action to be done; can be either 'check' or 'result'.
 */
function drush_langcheck_validate($action = 'result') {
  $action = drupal_strtolower($action);
  if (($action != 'check') && ($action != 'result')) {
    return drush_set_error('langcheck', dt("Invalid action; use either 'result' (the default) or 'check'."));
  }
}

/**
 * Implements drush_hook_COMMAND().
 *
 * @param $action
 *   Action to be done; can be either 'check' or 'result'.
 */
function drush_langcheck($action = 'result') {
  switch (drupal_strtolower($action)) {
    case 'check':
      module_load_include('inc', 'langcheck');
      $sandbox = array();
      langcheck_check_initialize($sandbox);
      if ($sandbox['results']['count_languages']) {
        $settings = variable_get('langcheck_settings', _langcheck_default_values('langcheck_settings'));
        $settings['check_batch']['num_of_files'] = 0;
        $settings['check_batch']['num_of_langs'] = 0;
        langcheck_check_doit($sandbox, $settings);
        langcheck_check_result_summary($sandbox);
        variable_set('langcheck_check_batch_result', $sandbox['results']);
      }
      // No break: do display the results right after they have been
      // calculated.
    case 'result':
      $results = variable_get('langcheck_check_batch_result', NULL);
      if (!$results) {
        drush_print(dt("There is no calculated results stored yet. Try issuing 'drush langcheck check'."));
        return;
      }
      drush_print_table(_langcheck_drush_langcheck_result_table($results), TRUE);
      break;
  }
}

/**
 * Converts the results array to another to be displayed as a table.
 */
function _langcheck_drush_langcheck_result_table($results) {
  $row = 0;
  $rows = array();
  $rows[$row][] = dt('Module');
  foreach ($results['language_names'] as $langcode => $langname) {
    $rows[$row][] = $langname;
  }
  $rows[$row++][] = dt('Module state');
  foreach ($results['data_module'] as $module => $langs) {
    $excluded_module = in_array($module, $results['excluded_modules']);
    $detail = $results['sum_language'][$module];
    $rows[$row][] = $module;
    foreach ($langs as $langcode => $detail) {
      $rows[$row][] = ($excluded_module) ? '---.--%' : str_pad(sprintf('%0.2f%%', $detail['state'] * 100), 7, ' ', STR_PAD_LEFT);
    }
    $rows[$row][] = ($excluded_module) ? '---.--%' : str_pad(sprintf('%0.2f%%', $results['sum_language'][$module]['state'] * 100), 7, ' ', STR_PAD_LEFT);
    $row++;
  }
  $overall = 0;
  $rows[$row][] = dt('Language state');
  foreach ($results['sum_module'] as $langcode => $detail) {
    $overall += $detail['state'];
    $rows[$row][] = str_pad(sprintf('%0.2f%%', $detail['state'] * 100), 7, ' ', STR_PAD_LEFT);
  }
  $state = $overall / $results['count_languages'];
  $rows[$row++][] = str_pad(sprintf('%0.2f%%', ($overall / $results['count_languages']) * 100), 7, ' ', STR_PAD_LEFT);
  return $rows;
}
