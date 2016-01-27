<?php
/**
 * @file
 * Helpers for working with ctools functionality
 */

$plugin = array (
  'title' => t('Ctools'),
  'description' => t('Library for working with ctools.'),
  'xu_type' => 'library'
);

/**
 * Given an exportable object, set its status to enabled
 *
 * @param $export
 * @param $table
 */
function xu_ctools_export_ui_enable($export, $table) {
  $status = FALSE;
  if(isset($export->disabled)) {
    unset($export->disabled);
  }
  xu_ctools_export_ui_status($export, $table, $status);
}

/**
 * Given an exportable object, set its status to disabled
 *
 * @param $export
 * @param $table
 */
function xu_ctools_export_ui_disable($export, $table) {
  $status = TRUE;
  xu_ctools_export_ui_status($export, $table, $status);
}

/**
 * Internal function to tweak ctools export status
 *
 * @param $export
 * @param $table
 * @param $status
 */
function xu_ctools_export_ui_status($export, $table, $status) {
  $export->table = $table;
  $export->type = 'Normal';
  $export->export_type = 1;

  $schema = ctools_export_get_schema($table);
  $definition = $schema['export'];
  ctools_export_set_status($table, $export->{$definition['key']}, $status);
  context_invalidate_cache();
}


