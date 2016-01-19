<?php

/**
 * Implements hook_features_export().
 */
function translatable_content_panes_content_pane_content_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['translatable_content_panes_content_pane_content'][$component] = $component;
  }

  return array();
}

/**
 * Implements hook_features_export_options().
 */
function translatable_content_panes_content_pane_content_features_export_options() {
  $options = array();
  $content_panes = translatable_content_panes_get_panes();

  foreach ($content_panes as $pane_id => $pane_info) {
    // Only those with code content ticked can be exported!
    if (!empty($pane_info['code_content'])) {
      $options[$pane_id] = $pane_info['admin_title'];
    }
  }

  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function translatable_content_panes_content_pane_content_features_export_render($module_name, $data, $export = NULL) {
  $content_panes = translatable_content_panes_get_panes();

  $code = array('  $content_panes_content = array();', '');
  foreach ($data as $pane_id) {
    if (array_key_exists($pane_id, $content_panes)) {
      $full_pane = translatable_content_panes_get_pane($pane_id, TRUE);
      $pane_content = array_diff_key($full_pane, $content_panes[$pane_id]);

      $code[] = "  \$content_panes_content['{$pane_id}'] = " . features_var_export($pane_content, '  ') . ';';
    }
  }

  $code[] = '';
  $code[] = "  return \$content_panes_content;";

  return array('translatable_content_panes_content_pane_content' => implode("\n", $code));
}

/**
 * Implements hook_features_revert().
 */
function translatable_content_panes_content_pane_content_features_revert($module_name) {
  $module_panes = module_invoke($module_name, 'translatable_content_panes_content_pane_content');

  // Delete the content pane info from the DB so it'll be grabbed from the feature on next load.
  db_delete('translatable_content_pane')->condition('id', array_keys($module_panes))->execute();

  // We also need to remove the content - there are no revisions involved. Next load will re-populate the database.
  db_delete('translatable_content_pane_data')->condition('id', array_keys($module_panes))->execute();

  // Reset the 2 static caches for the pane info / content.
  drupal_static_reset('translatable_content_panes_get_panes');
  drupal_static_reset('translatable_content_panes_content');
}

/**
 * Implements hook_features_rebuild().
 */
function translatable_content_panes_content_pane_content_features_rebuild($module_name) {
  translatable_content_panes_content_pane_content_features_revert($module_name);
}
