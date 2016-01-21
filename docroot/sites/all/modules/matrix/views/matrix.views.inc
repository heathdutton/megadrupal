<?php
// $Id: matrix.views.inc,v 1.1.2.1 2011/01/18 10:10:05 aaron1234nz Exp $

/**
 * @file
 * Views handlers for matrix fields
 */

/**
 * Implements hook_field_views_data_alter().
 */
function matrix_field_views_data_alter(&$result, &$field, &$module) {
  if ($module != 'matrix') {
    return;
  }
  $field_name = $field['field_name'];

  $tables = array('field_data_' . $field_name);
  if (isset($result['field_revision_' . $field_name])) {
    $tables[] = 'field_revision_' . $field_name;
  }

  foreach ($tables as $table) {
    // There's no reason to be able to select the 'row' or 'col' value.
    unset($result[$table][$field_name . '_row']);
    unset($result[$table][$field_name . '_col']);
    // Nor does it make sense to sort by the individual matrix values, since the
    // grid is fixed on both axis.
    unset($result[$table][$field_name . '_value']['sort']);

    $result[$table][$field_name]['field']['handler'] = 'matrix_handler_field_field';
    $result[$table][$field_name . '_value']['filter']['handler'] = 'matrix_handler_filter';
  }
}

/**
 * Implements hook_views_handler().
 */
function matrix_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'matrix') .'/views',
    ),
    'handlers' => array(
      'matrix_handler_field_field' => array(
       'parent' => 'views_handler_field_field',
      ),
      'matrix_handler_filter' => array(
        'parent' => 'views_handler_filter_string',
      ),
    ),
  );
}
