<?php
/**
 * @file
 * The theme system, which controls the output of views jQuery Mobile.
 *
 */

/**
 * Display a view as a grid style.
 *
 * @todo
 * add the ability to use fields the way tables do
 *
 */
function template_preprocess_views_jqm_basic_grid(&$vars) {
  $view     = $vars['view'];
  $result   = $view->result;
  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;
  $grid_type = array('a', 'b', 'c', 'd', 'e');

  $columns  = $options['columns'];

  $rows = array();
  $row_indexes = array();

  $row = array();
  $col_count = 0;
  $row_count = 0;
  $count = 0;
  foreach ($vars['rows'] as $row_index => $item) {
    $count++;
    $row[] = $item;
    $row_indexes[$row_count][$col_count] = $row_index;
    $col_count++;
    if ($count % $columns == 0) {
      $rows[] = $row;
      $row = array();
      $col_count = 0;
      $row_count++;
    }
  }
  if ($row) {
    $rows[] = $row;
  }

  // Apply the row classes
  foreach ($rows as $row_number => $row) {
    foreach ($rows[$row_number] as $column_number => $item) {
      $column_classes = array();
      $column_classes[] = 'ui-block-' . $grid_type[($column_number)];
       if (isset($row_indexes[$row_number][$column_number]) && $column_class = $view->style_plugin->get_row_class($row_indexes[$row_number][$column_number])) {
        $column_classes[] = $column_class;
      }
      $vars['column_classes'][$row_number][$column_number] = implode(' ', $column_classes);
    }
  }
  $vars['rows'] = $rows;
  $vars['class'] = 'ui-grid-' . $grid_type[$columns - 2];
  
  if (!empty($handler->options['summary'])) {
    $vars['attributes_array'] = array('summary' => $handler->options['summary']);
  }  
  if (!empty($handler->options['data_theme'])) {
    $vars['row_attributes']['class'][] = 'ui-bar-' . $handler->options['data_theme'];
  }
  $vars['row_attributes']['class'][] = 'ui-bar';

}

/**
 * Display the view as an HTML list element
 *
 * @todo
 * add the other list types if possible
 *
 */
function template_preprocess_views_jqm_basic_list(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $handler  = $vars['view']->style_plugin;
  $options  = $handler->options;
    
  $class = explode(' ', $handler->options['class']);
  $class = array_map('views_clean_css_identifier', $class);

  $wrapper_class = explode(' ', $handler->options['wrapper_class']);
  $wrapper_class = array_map('views_clean_css_identifier', $wrapper_class);

  $vars['class'] = implode(' ', $class);
  $vars['wrapper_class'] = implode(' ', $wrapper_class);
  
  $attributes = array();
  $attributes['data-role'] = 'listview';
  if($options['searchable']) {
    $attributes['data-filter'] = "true";
  }
  if ($vars['class']) {
    $attributes['class'] = $vars['class'];
  }
  
  $vars['wrapper_prefix'] = '';
  $vars['wrapper_suffix'] = '';
  $vars['list_type_prefix'] = '<' . $handler->options['type'] . ' ' . drupal_attributes($attributes) . '>';
  $vars['list_type_suffix'] = '</' . $handler->options['type'] . '>';
  if ($vars['wrapper_class']) {
    $vars['wrapper_prefix'] = '<div class="' . $vars['wrapper_class'] . '">';
    $vars['wrapper_suffix'] = '</div>';
  }
  
  $classes = array();
  $vars['attributes_array'] = array();
  // Set up striping values.
  $count = 0;
  $max = count($rows);
  foreach ($rows as $id => $row) {
    $count++; 
    if (!is_numeric($id)){
      $vars['attributes_array'][$id]['data-role'] = "list-divider";
    }
    $classes[$id][] = 'views-row';
    $classes[$id][] = 'views-row-' . $count;
    $classes[$id][] = 'views-row-' . ($count % 2 ? 'odd' : 'even');
    if ($count == 1) {
      $classes[$id][] = 'views-row-first';
    }
    if ($count == $max) {
      $classes[$id][] = 'views-row-last';
    }

    if ($row_class = $view->style_plugin->get_row_class($id)) {
      $classes[$id][] = $row_class;
    }

    // Flatten the classes to a string for each row for the template file.
    $vars['attributes_array'][$id]['class'] = implode(' ', $classes[$id]);
  }
}
