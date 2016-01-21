<?php
/**
 * @file
 * openacademy_events.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function openacademy_events_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|openacademy_event|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'openacademy_event';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'burr';
  $ds_layout->settings = array(
    'css_id' => '',
    'class' => '',
    'sticky' => 0,
    'layout' => 'burr',
    'hide_sidebars' => 0,
    'pipeline' => 'standard',
    'hide_empty_regions' => FALSE,
    'ds_panels' => TRUE,
    'did' => '2',
    'theme' => 'burr',
  );
  $export['node|openacademy_event|teaser'] = $ds_layout;

  return $export;
}
