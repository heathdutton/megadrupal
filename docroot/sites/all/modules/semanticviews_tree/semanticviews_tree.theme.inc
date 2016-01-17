<?php
/**
 * @file
 * Theming callbacks.
 */

/**
 * Implements template_preprocess_HOOK() for semanticviews_tree_view_tree.
 */
function template_preprocess_semanticviews_tree_view_tree(&$variables) {
  $view = $variables['view'];
  $options = $variables['options'];

  // Preprocess as Semantic Views would (guarding for list options).
  $variables['options']['list'] = array('element_type' => '', 'class' => '');
  module_load_include('inc', 'semanticviews', 'semanticviews.theme');
  template_preprocess_semanticviews_view_unformatted($variables);
  unset($variables['list_element']);
  unset($variables['list_attributes']);

  // Add container variables.
  $variables['container_element'] = check_plain($variables['options']['container']['element_type']);
  $variables['container_attributes'] = array();
  if ($variables['options']['container']['class']) {
    $variables['container_attributes']['class'] = $variables['options']['container']['class'];
  }

  // Populate subtrees.
  $subtrees = array();
  $main_field = $options['tree']['main_field'];
  $parent_field = $options['tree']['parent_field'];
  $row_indices = array();
  $style_plugin = $view->style_plugin;
  if (!empty($main_field) && !empty($parent_field)) {
    // Collect row information.
    $row_parents = array();
    foreach ($view->result as $i => $record) {
      $record_id = $style_plugin->get_field($i, $main_field);
      $record_parent = $style_plugin->get_field($i, $parent_field);
      if ($record_parent === '') {
        // Use NULL as parent if none found.
        $record_parent = NULL;
      }
      // Track record parent.
      $row_parents[$record_id] = $record_parent;
      // Track row number.
      $row_indices[$record_id] = $i;
    }
    // Build subtrees.
    foreach ($row_parents as $record_id => $record_parent) {
      // Alter parent if orphaned and if set to show as root.
      if (isset($record_parent) && !array_key_exists($record_parent, $row_parents) && !empty($options['tree']['orphan_as_root'])) {
        $record_parent = NULL;
      }
      // Insert record into subtree.
      $subtrees[$record_parent][] = $record_id;
    }
  }
  // Construct hierarchy.
  $row_tree = array();
  if (!empty($subtrees[NULL])) {
    $row_tree = semanticviews_tree_build_row_tree($row_indices, $subtrees);
  }
  $variables['row_tree'] = $row_tree;
}

/**
 * Recursively populate a row tree given row index map and subtrees.
 */
function semanticviews_tree_build_row_tree($row_indices, $subtrees, $parent = NULL) {
  $tree = array();
  if (!empty($subtrees[$parent])) {
    // Keep records and remove from subtrees to prevent infinite recursion.
    $records = $subtrees[$parent];
    unset($subtrees[$parent]);
    foreach ($records as $i => $record) {
      // Add record to tree.
      $tree[$i] = array('id' => $row_indices[$record]);
      // Mark the record as root.
      if (!isset($parent)) {
        $tree[$i]['root'] = TRUE;
      }
      // Build subtree if exists.
      if (!empty($subtrees[$record])) {
        $subtree = semanticviews_tree_build_row_tree($row_indices, $subtrees, $record);
        if (!empty($subtree)) {
          $tree[$i]['below'] = $subtree;
        }
      }
      // Mark the record as leaf if empty subtree.
      if (empty($tree[$i]['below'])) {
        $tree[$i]['leaf'] = TRUE;
      }
    }
  }
  return $tree;
}

/**
 * Implements template_process_HOOK() for semanticviews_tree_view_tree.
 */
function template_process_semanticviews_tree_view_tree(&$variables) {
  // Renders row tree into $rows_content.
  $variables['rows_content'] = semanticviews_tree_render_row_tree($row_tree = $variables['row_tree'], $variables['rows'], $variables);
}

/**
 * Recursively renders a row tree.
 */
function semanticviews_tree_render_row_tree($row_tree, $rows, $variables) {
  // Collect theme variables.
  $theme_variables = array();
  foreach (array('container_element', 'container_attributes', 'row_element', 'row_attributes') as $variable) {
    $theme_variables[$variable] = $variables[$variable];
  }
  // Collect rows.
  $rows_array = array();
  $root_class = $variables['options']['row']['root_class'];
  $leaf_class = $variables['options']['row']['leaf_class'];
  foreach ($row_tree as $row_item) {
    $id = $row_item['id'];
    // Render row content.
    $row_content = $rows[$id];
    if (!empty($theme_variables['row_attributes'][$id]['class'])) {
      $theme_variables['row_attributes'][$id]['class'] = (array) $theme_variables['row_attributes'][$id]['class'];
    }
    if (!empty($row_item['root']) && !empty($root_class)) {
      $theme_variables['row_attributes'][$id]['class'][] = $root_class;
    }
    if (!empty($row_item['leaf']) && !empty($leaf_class)) {
      $theme_variables['row_attributes'][$id]['class'][] = $leaf_class;
    }
    // Render subtree below.
    if (!empty($row_item['below'])) {
      $row_content .= semanticviews_tree_render_row_tree($row_item['below'], $rows, $variables);
    }
    // Render into a row.
    $rows_array[$id] = $row_content;
  }
  $theme_variables['rows'] = $rows_array;
  // Render rows using subtree theme.
  return theme('semanticviews_tree_subtree', $theme_variables);
}
