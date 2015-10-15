<?php

function template_preprocess_views_wookmark_plugin_style(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];

  $options = $view->style_plugin->options;

  // Generate unique view id
  $vars['views_wookmark_id'] = 'views_wookmark_'. $view->name .'_'. $view->current_display;
  $options['container_id'] = $vars['views_wookmark_id'];

  $vars['list_class'] = 'views-wookmark-grid-list';

  $vars['classes_array'] = array();
  $vars['classes'] = array();
  $default_row_class = isset($options['default_row_class']) ? $options['default_row_class'] : FALSE;
  $row_class_special = isset($options['row_class_special']) ? $options['row_class_special'] : FALSE;

  // Set up striping values.
  $count = 0;
  $max = count($rows);
  foreach ($rows as $id => $row) {
    $count++;
    if ($default_row_class) {
      $vars['classes'][$id][] = 'views-row';
      $vars['classes'][$id][] = 'views-row-' . $count;
    }
    if ($row_class_special) {
      $vars['classes'][$id][] = 'views-row-' . ($count % 2 ? 'odd' : 'even');
      if ($count == 1) {
        $vars['classes'][$id][] = 'views-row-first';
      }
      if ($count == $max) {
        $vars['classes'][$id][] = 'views-row-last';
      }
    }

    if ($row_class = $view->style_plugin->get_row_class($id)) {
      $vars['classes'][$id][] = $row_class;
    }

    // Flatten the classes to a string for each row for the template file.
    $vars['classes_array'][$id] = isset($vars['classes'][$id]) ? implode(' ', $vars['classes'][$id]) : '';
  }

  // Sanitize all the options
  $options_numeric = array('itemWidth', 'flexibleWidth', 'offset', 'resizeDelay');
  $options_bool = array('autoResize', 'fillEmptySpace');
  foreach($options as $option_key => $option_value) {
    if (in_array($option_key, $options_numeric)) {
      $options[$option_key] = (int) $option_value;
    }
    if (in_array($option_key, $options_bool)) {
      $options[$option_key] = (bool) $option_value;
    }
  }

  // Try to load the library and check if that worked.
  if (($library = libraries_load('wookmark')) && !empty($library['loaded'])) {
    if (!empty($vars['options']['default_css'])) {
      drupal_add_css(drupal_get_path('module', 'views_wookmark') . '/css/views_wookmark.css');
    }
    drupal_add_js(drupal_get_path('module', 'views_wookmark') . '/js/views_wookmark.js');
    drupal_add_js(array('views_wookmark' => $options), 'setting');
  }
}