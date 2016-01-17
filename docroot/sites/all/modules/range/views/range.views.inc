<?php

/**
 * @file
 * Implementation of Views API.
 */

/**
 * Implements hook_views_data().
 */
function range_views_data() {
  $data = array();

  foreach (field_info_fields() as $field) {
    if ($field['storage']['type'] != 'field_sql_storage') {
      continue;
    }
    if ($field['module'] != 'range') {
      continue;
    }

    _range_views_data_filter_handler($data, $field);
    _range_views_data_argument_handler($data, $field);
  }

  return $data;
}

/**
 * Helper function. Returns misc data for a given field.
 *
 * This function returns misc field data, that is required by various Views
 * plugins. Data includes field label, current and revision table names, field
 * columns.
 *
 * Field data is being statically cached.
 *
 * @param array $field
 *   Field definition.
 *
 * @return array
 *   Field data array containing:
 *     - Field label
 *     - Name of the table that contains current revision data for a given field
 *     - Name of the table that contains historical data for a given field
 *     - Array of field data columns keyed by column name.
 */
function _range_views_data_base_information(array $field) {
  static $information = array();

  if (empty($information[$field['field_name']])) {
    list($label) = field_views_field_label($field['field_name']);
    $add_fields = array();
    foreach ($field['columns'] as $column_name => $attributes) {
      $add_fields[$column_name] = _field_sql_storage_columnname($field['field_name'], $column_name);
    }

    $information[$field['field_name']] = array(
      $label,
      _field_sql_storage_tablename($field),
      _field_sql_storage_revision_tablename($field),
      $add_fields,
    );
  }

  return $information[$field['field_name']];
}

/**
 * Provides views filter handler data.
 *
 * @param array $data
 *   Views data array.
 * @param array $field
 *   Field definition.
 */
function _range_views_data_filter_handler(array &$data, array $field) {
  list($label, $current_table, $revision_table, $add_fields) = _range_views_data_base_information($field);

  $base = array(
    'handler' => 'range_handler_filter_numeric_range',
    'field_name' => $field['field_name'],
    'allow empty' => FALSE,
    'additional fields' => $add_fields,
  );

  $data[$current_table][$field['field_name']]['filter'] = array(
    'title' => t('@label range filter', array('@label' => $label)),
    'table' => $current_table,
  ) + $base;
  $data[$revision_table][$field['field_name'] . '-' . 'revision_id']['filter'] = array(
    'title' => t('@label range filter (historical data)', array('@label' => $label)),
    'table' => $revision_table,
  ) + $base;
}

/**
 * Provides views argument handler data.
 *
 * @param array $data
 *   Views data array.
 * @param array $field
 *   Field definition.
 */
function _range_views_data_argument_handler(array &$data, array $field) {
  list($label, $current_table, $revision_table, $add_fields) = _range_views_data_base_information($field);

  $base = array(
    'handler' => 'range_handler_argument_numeric_range',
    'field_name' => $field['field_name'],
    'additional fields' => $add_fields,
    'empty field name' => t('- No value -'),
    'numeric' => TRUE,
  );

  $data[$current_table][$field['field_name']]['argument'] = array(
    'title' => t('@label range argument', array('@label' => $label)),
    'table' => $current_table,
  ) + $base;
  $data[$revision_table][$field['field_name'] . '-' . 'revision_id']['argument'] = array(
    'title' => t('@label range argument (historical data)', array('@label' => $label)),
    'table' => $revision_table,
  ) + $base;
}
