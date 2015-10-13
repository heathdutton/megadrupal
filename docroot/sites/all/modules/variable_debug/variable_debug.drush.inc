<?php

/**
 * @file
 * Drush integration for the Variable Debug module.
 */

/**
 * Implementation of hook_drush_help().
 */
function variable_debug_drush_help($section) {
  switch ($section) {
    case 'meta:variable-debug:title':
      return dt('Variable Debug commands');
    case 'meta:variable-debug:summary':
      return dt('Checks your variable table for rouge and orphaned variables.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function variable_debug_drush_command() {
  $items['variable-debug'] = array(
    'description' => 'Checks your variable table for rouge and orphaned variables.',
    'arguments' => array(
      'count' => 'Number of large variables to return.'
    ),
    'options' => array(
      'large' => 'Display the large variables report.',
      'abandoned' => 'Display the abandoned variables report.',
      'noheader' => 'Do not display table header(s).',
    ),
    'examples' => array(
      'drush variable-debug' => 'Default output, large and abandoned variable reports.',
      'drush variable-debug --large 20' => 'Large variable report, showing top 20 largest variables.',
      'drush variable-debug --abandoned --noheader' => 'Abandoned variable report, excluding the table header.',
    ),
    'aliases' => array('vdebug'),
  );

  return $items;
}

/**
 * Command callback.
 */
function drush_variable_debug($count = NULL) {
  module_load_include('inc', 'variable_debug', 'variable_debug.report');
  if (drush_get_option('large') || (!drush_get_option('large') && !drush_get_option('abandoned'))) {
    drush_variable_debug_large_variables(($count > 0) ? $count : VARIABLE_DEBUG_LARGE_LIMIT);
  }
  if (drush_get_option('abandoned') || (!drush_get_option('large') && !drush_get_option('abandoned'))) {
    drush_variable_debug_abandoned_variables();
  }
}

/**
 * Output list of large variables.
 */
function drush_variable_debug_large_variables($count) {
  // Output formatted table.
  $show_header = (drush_get_option('noheader') ? FALSE : TRUE);
  $rows = array();
  if ($show_header) {
    $rows[] = array(dt('Variable name'), dt('Size'), dt('Drupal.org issue'));
  }
  $rows = array_merge($rows, variable_debug_get_large_rows($count, FALSE));
  drush_print_table($rows, $show_header);
}

/**
 * Output list of abandoned variables.
 */
function drush_variable_debug_abandoned_variables() {
  // Output formatted table.
  $show_header = (drush_get_option('noheader') ? FALSE : TRUE);
  $rows = array();
  if ($show_header) {
    $rows[] = array(t('Variable name'), 'size' => t('Size'));
  }
  $rows = array_merge($rows, variable_debug_get_abandoned_rows());
  drush_print_table($rows, $show_header);
}
