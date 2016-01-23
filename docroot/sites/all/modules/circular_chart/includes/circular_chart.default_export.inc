<?php
/**
 * @file
 * File stores the hooks for the Ctools exportables.
 */

/**
 * Implements hook_default_circular_chart_preset().
 */
function circular_chart_default_circular_chart_preset() {
  $export = array();

  $circular_chart_preset = new stdClass();
  $circular_chart_preset->api_version = 1;
  $circular_chart_preset->name = 'default_circular_chart';
  $circular_chart_preset->label = 'Default Circular Chart';
  $circular_chart_preset->data = array(
    'circle_radius' => 60,
    'circle_width' => 10,
    'circle_bg_color' => '#4B253A',
    'circle_fg_color' => '#D3B6C6',
    'circle_anim_duration' => 500,
    'circle_wrapper_class' => 'circles-wrp',
  );
  $export['default_circular_chart'] = $circular_chart_preset;

  return $export;
}
