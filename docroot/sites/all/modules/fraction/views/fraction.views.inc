<?php

/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_field_views_data_alter().
 */
function fraction_field_views_data_alter(&$result, $field, $module) {

  // Only operate on fields defined by the Fraction module.
  if ($module != 'fraction') {
    return;
  }

  // Look up the field's most used instance label.
  list($field_label, $other_labels) = field_views_field_label($field['field_name']);

  // Build a list of additional fields for the sort and filter handlers.
  $additional_fields = array(
    $field['field_name'] . '_numerator',
    $field['field_name'] . '_denominator',
  );

  // Iterate through the fields.
  foreach ($field['storage']['details']['sql'] as $type => $tables) {
    foreach ($tables as $table_name => $columns) {

      // Determine the field name.
      $field_name = $field['field_name'];
      if ($type == FIELD_LOAD_REVISION) {
        $field_name .= '-revision_id';
      }

      // Skip if the field does not exist in the result.
      // For example, entities without revisions will not have a revision field
      // in the result, even though they have a revision field in the schema.
      if (empty($result[$table_name][$field_name])) {
        continue;
      }

      // Create an alias to the field column.
      $field_column = $result[$table_name][$field_name];

      // Override the field handler so that we can provide our own custom
      // click sort method (that uses the fraction's decimal equivalent).
      $result[$table_name][$field_name]['field']['handler'] = 'fraction_handler_field';
      $result[$table_name][$field_name]['field']['additional fields'] = $additional_fields;

      // Create a new decimal column with custom sort and filter handlers.
      $column_name = $field['field_name'] . '_decimal';
      $result[$table_name][$column_name] = array(
        'group' => t($field_column['group']),
        'title' => t($field_label . ' (decimal)'),
        'title short' => t($field_label . ':decimal'),
        'help' => t('Decimal equivalent of Fraction field for sorting and filtering. ' . $field_column['help']),
        'sort' => array(
          'handler' => 'fraction_handler_sort_decimal',
          'additional fields' => $additional_fields,
        ),
        'filter' => array(
          'handler' => 'fraction_handler_filter_decimal',
          'additional fields' => $additional_fields,
        )
      );
    }
  }
}

