<?php

/**
 * @file
 * Contains a pre-process hook for 'views_view_list'.
 */

/**
 * Implements hook_preprocess_views_view_list().
 */
function semantic_ui_omega_preprocess_views_view_list(&$variables) {
  //@TODO - handle if class is already in type prefix
  $variables['list_type_prefix'] = str_replace('>', ' class="ui list">',$variables['list_type_prefix']);
  
  // For some reason views puts row classes into the classes array. Instead of
  // classes arrays we should always use proper attributes arrays and never
  // abuse the default versions of those for row classes. Instead, we should use
  // a custom variable for that, which is exactly what we convert it to here.
  foreach ($variables['rows'] as $delta => $row) {
    $variables['row_attributes_array'][$delta] = isset($variables['row_attributes_array'][$delta]) ? $variables['row_attributes_array'][$delta] : array();

    if (!empty($variables['classes_array'][$delta])) {
      $variables['row_attributes_array'][$delta]['class'] = explode(' ', $variables['classes_array'][$delta]);
    }
  }
}
