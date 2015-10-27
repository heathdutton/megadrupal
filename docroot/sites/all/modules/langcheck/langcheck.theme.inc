<?php

/**
 * @file
 * Theme functions.
 */

/**
 * Returns HTML for the results of a language check.
 *
 * @param $variables
 *   An associative array containing:
 *   - language_names: Array of table headers.
 *   - count_languages: Number of columns with valuable information.
 *   - data_module: Array of information about modules (ie. how many strings
 *     does a module have and how many of them is translated to a language.)
 *   - sum_language: Array of overall information about languages (ie. an
 *     average of the previous one.)
 *   - sum_module: Array of overall information about modules (ie. the average
 *     translation status of the modules).
 *
 * @ingroup themeable
 */
function theme_langcheck_check_batch_result($variables) {
  drupal_add_css(drupal_get_path('module', 'langcheck') . '/langcheck.css');
  $table = array(
    'header' => array(t('Module')),
    'rows' => array(),
    'attributes' => array('id' => 'langcheck-result-table'),
  );
  foreach ($variables['language_names'] as $langcode => $langname) {
    $table['header'][] = $langname;
  }
  if ($variables['count_languages'] > 1) {
    $table['header'][] = t('Module state');
  }
  foreach ($variables['data_module'] as $module => $langs) {
    $excluded_module = in_array($module, $variables['excluded_modules']);
    $detail = $variables['sum_language'][$module];
    $row = array();
    // Column: Module name.
    $row[] = array(
      'data'   => $module,
      'header' => TRUE,
      'class'  => array(0 => (($excluded_module) ? 'excluded' : 'langcheck-state-' . floor($detail['state'] / 0.1))),
      'title'  => t('Total: @total, untranslated: @untranslated', array('@total' => $detail['total'], '@untranslated' => $detail['untranslated'])),
    );
    // Columns: Languages one by one.
    foreach ($langs as $langcode => $detail) {
      if ($excluded_module) {
        $row[] = '---.--%';
      }
      else {
        $row[] = array(
          'data' => sprintf('%0.2f%%', $detail['state'] * 100),
          'class' => array('langcheck-state-' . floor($detail['state'] / 0.1)),
          'title' => t('Total: @total, untranslated: @untranslated', array('@total' => $detail['total'], '@untranslated' => $detail['untranslated'])),
        );
      }
    }
    // Column: Last column is the module summary.
    if ($variables['count_languages'] > 1) {
      if ($excluded_module) {
        $row[] = '---.--%';
      }
      else {
        $detail = $variables['sum_language'][$module];
        $row[] = array(
          'data' => sprintf('%0.2f%%', $detail['state'] * 100),
          'class' => array('langcheck-state-' . floor($detail['state'] / 0.1)),
          'title' => t('Total: @total, untranslated: @untranslated', array('@total' => $detail['total'], '@untranslated' => $detail['untranslated'])),
        );
      }
    }
    $table['rows'][] = ($excluded_module) ? array('title' => t('Excluded'), 'class' => array('excluded'), 'data' => $row) : $row;
  }
  $overall = 0;
  $row = array();
  $row[] = array('header' => TRUE, 'data' => t('Language state'));
  foreach ($variables['sum_module'] as $langcode => $detail) {
    $overall += $detail['state'];
    $row[] = array(
      'data' => sprintf('%0.2f%%',  $detail['state'] * 100),
      'class' => array('langcheck-state-' . floor($detail['state'] / 0.1)),
      'title' => t('Total: @total, untranslated: @untranslated', array('@total' => $detail['total'], '@untranslated' => $detail['untranslated'])),
    );
  }
  if ($variables['count_languages'] > 1) {
    $state = $overall / $variables['count_languages'];
    $row[] = array(
      'data' => sprintf('%0.2f%%', $state * 100),
      'class' => array('langcheck-state-' . floor($state / 0.1)),
    );
  }
  $table['rows'][] = $row;
  // If too many languages are enabled, the table becomes too wide. Enclose it
  // in a scrollable div.
  return '<div id="langcheck-result-table-wrapper">' . theme('table', $table) . '</div>';
}
