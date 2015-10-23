<?php

/**
 * @file
 *   Drush support for dba.
 */

/**
 * Implementation of hook_drush_command().
 */
function dba_drush_command() {
  $items = array();
  $items['dba-sqlreport'] = array(
    'callback' => 'dba_drush_callback_sqlreport',
    'description' => 'Generate a statistical SQL server report.',
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function dba_drush_help($section) {
  switch ($section) {
    case 'drush:dba':
      return dt("Lists database tables. Usage: 'drush dba'.");
    case 'drush:sqlreport':
      return dt("Generates a statistical report about your SQL database server. Usage: 'drush dba sqlreport'.");
  }
}

function dba_drush_callback_sqlreport() {
  require_once(drupal_get_path('module', 'dba') . '/dba.report.inc');
  $output = dba_report_page(TRUE);
  echo $output;
}
