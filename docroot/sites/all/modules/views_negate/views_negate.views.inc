<?php

/**
 * @file
 * Added negate handler to arguments in this file.
 */

/**
 * Implements hook_field_views_data_alter().
 *
 * Change the handler of list and text fields.
 */
function views_negate_field_views_data_alter(array &$result, array $field, $module) {
  if ($field['type'] == 'list_text') {
    $table_name = 'field_data_' . $field['field_name'];
    $field_name = $field['field_name'] . '_value';
    $result[$table_name][$field_name]['argument']['handler'] = 'ViewsNegateHandlerArgumentFieldListString';
  }
  if ($field['type'] == 'text') {
    $table_name = 'field_data_' . $field['field_name'];
    $field_name = $field['field_name'] . '_value';
    $result[$table_name][$field_name]['argument']['handler'] = 'ViewsNegateHandlerArgumentString';
  }
}
