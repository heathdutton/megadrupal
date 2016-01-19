<?php

/**
 * Implements hook_features_export().
 */
function translatable_content_panes_content_pane_features_export($data, &$export, $module_name) {
  $pipe = array();

  foreach ($data as $pane_id) {
    $export['features']['translatable_content_panes_content_pane'][$pane_id] = $pane_id;

    // If it's a code pane, add in the content.
    $pane = translatable_content_panes_get_pane($pane_id);
    if (!empty($pane['code_content'])) {
      $pipe['translatable_content_panes_content_pane_content'][$pane_id] = $pane_id;
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function translatable_content_panes_content_pane_features_export_options() {
  $options = array();
  $content_panes = translatable_content_panes_get_panes();

  foreach ($content_panes as $pane_id => $pane_info) {
    $options[$pane_id] = $pane_info['admin_title'];
  }

  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function translatable_content_panes_content_pane_features_export_render($module_name, $data, $export = NULL) {
  $content_panes = translatable_content_panes_get_panes();

  $code = array('  $content_panes = array();', '');
  foreach ($data as $pane_id) {
    if (array_key_exists($pane_id, $content_panes)) {
      $pane_info = $content_panes[$pane_id];
      $code[] = "  \$content_panes['{$pane_id}'] = " . features_var_export($pane_info, '  ') . ';';
    }
  }

  $code[] = '';
  $code[] = "  return \$content_panes;";

  return array('translatable_content_panes_content_pane' => implode("\n", $code));
}

/**
 * Implements hook_features_revert().
 */
function translatable_content_panes_content_pane_features_revert($module_name) {
  $module_panes = module_invoke($module_name, 'translatable_content_panes_content_pane');

  db_delete('translatable_content_pane')->condition('id', array_keys($module_panes))->execute();

  drupal_static_reset('translatable_content_panes_get_panes');
}

/**
 * Implements hook_features_rebuild().
 */
function translatable_content_panes_content_pane_features_rebuild($module_name) {
  translatable_content_panes_content_pane_features_revert($module_name);
}

/**
 * Implements hook_features_pipe_COMPONENT_alter().
 */
function translatable_content_panes_features_pipe_page_manager_handlers_alter(&$pipe, $data, $export) {
  ctools_include('export');
  $handlers = ctools_export_load_object('page_manager_handlers', 'names', $data);
  foreach ($handlers as $handler) {
    $panes = panels_load_display($handler->did);
    _translatable_content_panes_check_panels_for_dependencies($panes->content, $pipe);
  }
}

/**
 * Implements hook_features_pipe_COMPONENT_alter().
 */
function translatable_content_panes_features_pipe_panels_mini_alter(&$pipe, $data, $export) {
  foreach ($data as $mini_panel_name) {
    $mini_panel = panels_mini_load($mini_panel_name);
    _translatable_content_panes_check_panels_for_dependencies($mini_panel->display->content, $pipe);
  }
}

/**
 * Helper to recursively check for a translatable content pane in a Panel.
 */
function _translatable_content_panes_check_panels_for_dependencies($panes, &$features_pipe) {
  // Sometimes comes in as NULL.
  if (empty($panes)) {
    return;
  }

  foreach ($panes as $pane) {
    if ($pane->type == 'translatable_content_pane') {
      $features_pipe['translatable_content_panes_content_pane'][] = $pane->configuration['id'];
    }
    else if ($pane->type == 'panels_mini') {
      $mini_panel = panels_mini_load($pane->subtype);
      _translatable_content_panes_check_panels_for_dependencies($mini_panel->display->content, $features_pipe);
    }
  }
}
