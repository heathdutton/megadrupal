<?php
/**
 * @file
 * fancy_content_picture.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function fancy_content_picture_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_content|picture|default';
  $ds_layout->entity_type = 'fancy_content';
  $ds_layout->bundle = 'picture';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_reset';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_fancy_picture',
      ),
    ),
    'fields' => array(
      'field_fancy_picture' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => TRUE,
    'layout_link_attribute' => FALSE,
    'layout_link_custom' => '',
  );
  $export['fancy_content|picture|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_content|picture|inline_entity_form_preview';
  $ds_layout->entity_type = 'fancy_content';
  $ds_layout->bundle = 'picture';
  $ds_layout->view_mode = 'inline_entity_form_preview';
  $ds_layout->layout = 'ds_reset';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_fancy_picture',
      ),
    ),
    'fields' => array(
      'field_fancy_picture' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => TRUE,
    'layout_link_attribute' => FALSE,
    'layout_link_custom' => '',
  );
  $export['fancy_content|picture|inline_entity_form_preview'] = $ds_layout;

  return $export;
}
