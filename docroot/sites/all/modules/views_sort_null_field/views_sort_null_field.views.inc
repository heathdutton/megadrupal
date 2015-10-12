<?php

/**
 * @file views_sort_null_field.views.inc
 * Contains Views hooks.
 */

/**
 * Implements hook_field_views_data_alter().
 */
function views_sort_null_field_field_views_data_alter(&$result, $field, $module) {
  // Bail for weird fields with no columns.
  if (empty($field['columns'])) {
    return;
  }

  $table = _field_sql_storage_tablename($field);
  // Bail for fields that don't use a standard table.
  if (!isset($result[$table])) {
    return;
  }

  // Load all the fields from the table by default.
  $additional_fields = array_values($field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]);

  // Build up an array of the labels we need for the bundles this field is on,
  // and also check the instances to see if they are all set to 'required'.
  $bundles_names = array();
  $required = TRUE;
  foreach ($field['bundles'] as $entity_type => $bundles) {
    foreach ($bundles as $bundle_name) {
      $bundles_names[] = t('@entity:@bundle', array('@entity' => $entity_type, '@bundle' => $bundle_name));
    }

    // Determine whether this instance is set to 'required', and keep track of
    // whether they all are. $required is initialized to TRUE, so if after
    // ANDing with the required setting of each instance, it is still TRUE, then
    // all instances are set to 'required'.
    $instance = field_info_instance($entity_type, $field['field_name'], $bundle_name);
    $required &= $instance['required'];
  }

  // If all instances are required, this sort is pointless, as no field value
  // will be empty. Bail.
  if ($required) {
    return;
  }

  foreach ($field['columns'] as $column_name => $attributes) {
    // Skip columns that can't be NULL: our filter is pointless for these.
    if (!empty($attributes['not null'])) {
      continue;
    }

    $base_field_name = _field_sql_storage_columnname($field['field_name'], $column_name);
    $nullsort_field_name = $base_field_name . '_nullsort';

    $column_real_name = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table][$column_name];

    list($label, $all_labels) = field_views_field_label($field['field_name']);
    $title = t('@label (!name:!column:null sort)', array(
      '@label' => $label,
      '!name' => $field['field_name'],
      '!column' => $column_name,
    ));
    $title_short = t('@label:!column:null sort', array(
      '@label' => $label,
      '!column' => $column_name,
    ));

    // Add a fake column to the field's table.
    $result[$table][$nullsort_field_name] = array(
      'group' => $result[$table][$base_field_name]['group'],
      'title' => $title,
      'title short' => $title_short,
      'help' => t('Null sort - Appears in: @bundles.', array('@bundles' => implode(', ', $bundles_names))),
    );

    // Add a sort handler to our column.
    $result[$table][$nullsort_field_name]['sort'] = array(
      'field' => $column_real_name,
      'table' => $table,
      'handler' => 'views_handler_sort_null_field',
      'additional fields' => $additional_fields,
      'field_name' => $field['field_name'],
    );
  }
}
