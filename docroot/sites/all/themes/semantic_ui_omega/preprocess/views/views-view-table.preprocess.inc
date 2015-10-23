<?php

/**
 * @file
 * Contains a pre-process hook for 'views_view_table'.
 */

/**
 * Implements hook_preprocess_views_view_table().
 */
function semantic_ui_omega_preprocess_views_view_table(&$variables) {
  $variables['classes_array'][] = 'ui celled sortable definition table segment';
  foreach ($variables['rows'] as $delta => $row) {
    $variables['row_attributes_array'][$delta] = isset($variables['row_attributes_array'][$delta]) ? $variables['row_attributes_array'][$delta] : array();
    if (!empty($variables['row_classes'][$delta])) {
      if (in_array('odd', $variables['row_classes'][$delta])) {
        $variables['row_classes'][$delta][] = 'positive';
      } else {
        $variables['row_classes'][$delta][] = 'negative';
      }
      $variables['row_attributes_array'][$delta]['class'] = $variables['row_classes'][$delta];
    }

    // Views tables have additional classes for each table column.
    foreach ($row as $field => $content) {
      $variables['field_attributes_array'][$field][$delta] = isset($variables['field_attributes'][$field][$delta]) ? $variables['field_attributes'][$field][$delta] : array();

      if (!empty($variables['field_classes'][$field][$delta])) {
        $variables['field_attributes_array'][$field][$delta]['class'] = explode(' ', $variables['field_classes'][$field][$delta]);
      }
    }
  }
}
