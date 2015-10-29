<?php
/**
 * @file
 * views-view-table.vars.php
 */

/**
 * Implements hook_preprocess_views_view_table().
 */
function wetkit_bootstrap_preprocess_views_view_table(&$variables) {
  bootstrap_include('wetkit_bootstrap', 'templates/system/table.vars.php');
  _bootstrap_table_add_classes($variables['classes_array'], $variables);
}
