<?php

/**
 * @file
 * Drush command hooks for the views_usage_audit module.
 */

/**
 * Implements hook_drush_command().
 */
function views_usage_audit_drush_command() {
  $items = array();

  $items['views-usage-audit-report-usage'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
    'description' => t('Show all views and displays that have not been executed to date.'),
  );

  $items['views-usage-audit-report-unused'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
    'description' => t('Show all views and displays that have not been executed to date.'),
  );

  $items['views-audit-truncate'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
    'description' => t('Remove all entries from the views_audit table'),
  );

  return $items;
}

/**
 * Command callback for the views-audit-report-unused command.
 */
function drush_views_usage_audit_report_unused() {

  $unused = _views_usage_audit_fetch_unused_views();

  foreach ($unused as $view) {
    drush_log(dt('Never executed display: !view:!display_names',
      array(
        '!view' => $view[0],
        '!display_names' => $view[1],
      )), 'warning'
    );
  }
}

/**
 * Command callback for the views-audit-report-usage command.
 */
function drush_views_usage_audit_report_usage() {
  $data = _views_usage_audit_fetch_audit_data();

  foreach ($data as $view => $displays) {
    drush_print('view: ' . $view);

    foreach ($displays as $display => $paths) {
      drush_print('display: ' . $display, 2);

      foreach ($paths as $path => $value) {
        drush_print('path: ' . $path, 4);
      }
    }
    drush_print('');
  }
}

/**
 * Command callback for the views-audit-truncate command.
 */
function drush_views_usage_audit_views_audit_truncate() {
  db_truncate('views_usage_audit')->execute();
  cache_clear_all('views_usage_audit_data', 'cache');
}
