<?php
/**
 * @file
 *
 * Implements Views hooks for custom field integration
 */

/**
 * Implements hook_field_views_data
 */
function vinfield_field_views_data($field) {
  $data = field_views_field_default_views_data($field);

  // Only expose these components as Views field handlers.
  $implemented = array(
    'vin' => 'views_handler_field',
    'year' => 'views_handler_field',
    'make' => 'views_handler_field',
    'model' => 'views_handler_field',
    'trim' => 'views_handler_field',
    'extra' => 'views_handler_field_serialized',
  );

  // Iterate over field defined tables.
  foreach ($data as &$table) {
    // Make sure the parent Views field is defined.
    if (isset($table[$field['field_name']]['field'])) {
      // Use the parent field definition as a template for component columns.
      $field_def = $table[$field['field_name']]['field'];

      // Remove 'additional fields' from the field definition. We don't
      // necessarily want all our sibling columns.
      unset($field_def['additional fields']);

      // Define the valid columns.
      $valid_columns = array();
      foreach ($implemented as $implement => $handler) {
        $column_name = $field['field_name'] . '_' . $implement;
        $valid_columns[$column_name] = $handler;
      }

      // Iterate over the field components.
      foreach ($table as $column_name => &$column) {
        if (empty($column['field']) && isset($valid_columns[$column_name])) {
          // Assign the default component definition.
          $column['field'] = $field_def;
          $column['field']['real field'] = $column_name;
          $column['field']['handler'] = $valid_columns[$column_name];

          // Assign human-friendly field labels for addressfield components.
          $field_labels = field_views_field_label($field['field_name']);
          $field_label = array_shift($field_labels);
        }
      }
    }
  }

  return $data;
}