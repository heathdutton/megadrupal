<?php

/**
 * @file
 * Provides Panels integration.
 */

/**
 * Implements hook_vefl_layouts().
 */
function ds_vefl_layouts() {
  $module_layouts = ds_get_layout_info();

  $layouts = array();
  foreach ($module_layouts as $id => $layout) {
    if (!empty($layout['module']) && $layout['module'] == 'panels') {
      continue;
    }
    $layouts[$id] = array(
      'title' => $layout['label'],
      'regions' => $layout['regions'],
      'module' => 'Display Suite',
      'info' => array(
        'path' => $layout['path'],
      ),
    );
  }
  return $layouts;
}

/**
 * Theme function for Views exposed form.
 *
 * Wraps form field into regions.
 *
 * @see _vefl_form_theme_functions()
 */
function vefl_theme_views_exposed_form_ds(&$vars) {
  $layout_id = $vars['form']['#layout']['layout_id'];

  // Prepare content to output in panels.
  $content = array();
  foreach ($vars['region_widgets'] as $region => $widgets) {
    $content[$region] = '';
    foreach ($widgets as $widget) {
      $content[$region] .= theme('views_exposed_widget', array('widget' => $widget));
    }
    $content[$region . '_wrapper'] = 'div';
    $content[$region . '_classes'] = '';
  }
  $content['layout_wrapper'] = 'div';
  $content['layout_attributes'] = '';
  $content['region_widgets'] = $vars['region_widgets'];

  drupal_add_css(drupal_get_path('module', 'ds') . '/layouts/' . $layout_id . '/' . $layout_id . '.css');

  // Outputs content in panels layout.
  return theme($layout_id, $content);
}

/**
 * A theme preprocess function for ds layout.
 *
 * Adds special classes for layout.
 */
function vefl_ds_preprocess_layout(&$vars) {
  if (!empty($vars['region_widgets'])) {
    $vars['classes_array'] = array('views-exposed-form', 'views-exposed-widgets');
  }
}
