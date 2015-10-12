<?php

/**
 * @file
 * Contains a pre-process hook for 'views_view_grid'.
 */

/**
 * Implements hook_preprocess_views_view_grid().
 */
function semantic_ui_omega_preprocess_views_view_grid(&$variables) {
  $options_count = array(1=>'one',2=>'two',3=>'three',4=>'four',5=>'five',6=>'six');
  $options = $variables['options'];
  $classes  = 'ui items '.$options_count[$options['columns']];

  $variables['show_divider'] = TRUE;
  if($variables['view']->current_display =='block'){
    $variables['show_divider'] = FALSE;
  }
  
  $options = $variables['view']->style_plugin->options;
  $columns = $options['columns'];
  foreach ($variables['rows'] as $delta => $row) {
    $variables['row_attributes_array'][$delta] = isset($variables['row_attributes_array'][$delta]) ? $variables['row_attributes_array'][$delta] : array();

    if (!empty($variables['row_classes'][$delta])) {
      $variables['row_attributes_array'][$delta]['class'] = explode(' ', $variables['row_classes'][$delta]);
    }
    $variables['row_attributes_array'][$delta]['class'][]=$classes;
    
    // Views tables have additional classes for each column.
    for ($column = 0; $column < $columns; $column++) {
      $variables['column_attributes_array'][$delta][$column] = isset($variables['column_attributes_array'][$delta][$column]) ? $variables['column_attributes_array'][$delta][$column] : array();
      if (!empty($variables['column_classes'][$delta][$column])) {
        $variables['column_attributes_array'][$delta][$column]['class'] = explode(' ', $variables['column_classes'][$delta][$column]);
      }
      $variables['column_attributes_array'][$delta][$column]['class'][]='column item';
    }
  }
}
