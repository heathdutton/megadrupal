<?php

/**
 * @file
 * Theme overrides for views.inc functions.
 */

/**
 * Implements template_preprocess_views_view().
 *
 * Remove views classes
 */
function bassline_preprocess_views_view(&$vars) {
  // Classes to remove.
  $remove = array(
    'view',
    'view-' . $vars['name'],
    'view-id-' . $vars['name'],
    'view-display-id-' . $vars['display_id'],
    'view-dom-id-' . $vars['dom_id'],
  );
  // Remove default classes.
  $vars['classes_array'] = array_diff($vars['classes_array'], $remove);
}


/**
 * Implements template_preprocess_views_view_unformatted().
 *
 * Remove views prefix on classes
 */
function bassline_preprocess_views_view_unformatted(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];
  $style = $view->style_plugin;
  $options = $style->options;

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
      $vars['classes'][$id][] = 'row-' . $count;
    }
    if ($row_class_special) {
      $vars['classes'][$id][] = ($count % 2) ? 'odd' : 'even';
      if ($count == 1) {
        $vars['classes'][$id][] = 'first';
      }
      if ($count == $max) {
        $vars['classes'][$id][] = 'last';
      }
    }

    if ($row_class = $view->style_plugin->get_row_class($id)) {
      $vars['classes'][$id][] = $row_class;
    }

    // Flatten the classes to a string for each row for the template file.
    $vars['classes_array'][$id] = isset($vars['classes'][$id]) ? implode(' ', $vars['classes'][$id]) : '';
  }
}
