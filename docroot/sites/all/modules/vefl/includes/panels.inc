<?php

/**
 * @file
 * Provides Panels integration.
 */

/**
 * Implements hook_vefl_layouts().
 */
function panels_vefl_layouts() {
  ctools_include('plugins', 'panels');
  $module_layouts = panels_get_layouts();

  $layouts = array();
  foreach ($module_layouts as $id => $layout) {
    $regions = panels_get_regions($layout, panels_new_display());
    $layouts['panels_' . $id] = array(
      'title' => $layout['title'],
      'regions' => $regions,
      'module' => 'Panels',
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
function vefl_theme_views_exposed_form_panels(&$vars) {
  ctools_include('plugins', 'panels');

  // Prepare content to output in panels.
  $content = array();
  foreach ($vars['region_widgets'] as $region => $widgets) {
    $content[$region] = '';
    foreach ($widgets as $widget) {
      $content[$region] .= theme('views_exposed_widget', array('widget' => $widget));
    }
  }

  // Outputs content in panels layout.
  $layout_name = substr($vars['form']['#layout']['layout_id'], 7);
  $panel_content = panels_print_layout(panels_get_layout($layout_name), $content);
  return '<div class="views-exposed-form views-exposed-widgets clearfix">' . $panel_content . '</div>';
}
