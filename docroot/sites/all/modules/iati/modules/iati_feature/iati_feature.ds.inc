<?php
/**
 * @file
 * iati_feature.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function iati_feature_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|iati_activity|map_popup';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'iati_activity';
  $ds_fieldsetting->view_mode = 'map_popup';
  $ds_fieldsetting->settings = array(
    'donor_field' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|iati_activity|map_popup'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function iati_feature_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|iati_activity|map_popup';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'iati_activity';
  $ds_layout->view_mode = 'map_popup';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_other_identifier',
        1 => 'field_iati_activity_description',
        2 => 'iati_activity_sector',
        3 => 'donor_field',
        4 => 'field_iati_location',
        5 => 'field_iati_activity_planned_date',
        6 => 'field_iati_activity_status',
      ),
    ),
    'fields' => array(
      'field_other_identifier' => 'ds_content',
      'field_iati_activity_description' => 'ds_content',
      'iati_activity_sector' => 'ds_content',
      'donor_field' => 'ds_content',
      'field_iati_location' => 'ds_content',
      'field_iati_activity_planned_date' => 'ds_content',
      'field_iati_activity_status' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
  );
  $export['node|iati_activity|map_popup'] = $ds_layout;

  return $export;
}

/**
 * Implements hook_ds_view_modes_info().
 */
function iati_feature_ds_view_modes_info() {
  $export = array();

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'map_popup';
  $ds_view_mode->label = 'Map Popup';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['map_popup'] = $ds_view_mode;

  return $export;
}
