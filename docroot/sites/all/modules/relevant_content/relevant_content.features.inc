<?php

/**
 * @file relevant_content.features.inc
 * Contains the implementions to enable Features Exportable settings
 */


/**
 * Implementation of hook_features_export().
 */
function relevant_content_block_features_export($data, &$export, $module_name) {
  $pipe = array();

  foreach ($data as $delta) {
    if ($settings = relevant_content_get_settings($delta)) {
      $export['features']['relevant_content_block'][$delta] = $delta;
    }
  }

  $export['dependencies'] += array(
    'features' => 'features',
    'taxonomy' => 'taxonomy',
  );

  return $pipe;
}


/**
 * Implementation of hook_features_export_options().
 */
function relevant_content_block_features_export_options() {
  $options = array();
  foreach (relevant_content_get_settings() as $block) {
    $options[$block['id']] = $block['id'];
  }
  return $options;
}


/**
 * Implementation of hook_features_export_options_render().
 */
function relevant_content_block_features_export_render($module_name, $data) {
  $items = array();

  foreach ($data as $delta) {
    $block = relevant_content_get_settings($delta);
    _features_sanitize($block);
    $items[$delta] = $block;
  }

  $code = '  return ' . features_var_export($items, '  ') . ';';
  return array('relevant_content_default_blocks' => $code);
}


/**
 * Implementation of hook_features_revert().
 */
function relevant_content_block_features_revert($module) {
  // Get the settings for the Feature module
  $blocks = module_invoke($module, 'relevant_content_default_blocks');

  // For each block, if there are any, delete any overriden settings
  if (!empty($blocks)) {
    foreach ($blocks as $block) {
      relevant_content_delete_block($block['id'], FALSE);
    }
  }

  // Reset the cache
  relevant_content_get_settings(NULL, TRUE);
}
