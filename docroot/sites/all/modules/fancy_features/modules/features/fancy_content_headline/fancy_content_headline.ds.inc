<?php
/**
 * @file
 * fancy_content_headline.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function fancy_content_headline_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_content|headline|default';
  $ds_layout->entity_type = 'fancy_content';
  $ds_layout->bundle = 'headline';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_headline',
        1 => 'field_fancy_textcolor',
        2 => 'field_fancy_style',
      ),
    ),
    'fields' => array(
      'field_headline' => 'ds_content',
      'field_fancy_textcolor' => 'ds_content',
      'field_fancy_style' => 'ds_content',
    ),
    'classes' => array(
      'layout_class' => array(
        'content-[fancy_content:id]' => 'content-[fancy_content:id]',
      ),
    ),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_content|headline|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_content|headline|inline_entity_form_preview';
  $ds_layout->entity_type = 'fancy_content';
  $ds_layout->bundle = 'headline';
  $ds_layout->view_mode = 'inline_entity_form_preview';
  $ds_layout->layout = 'ds_reset';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_headline',
      ),
    ),
    'fields' => array(
      'field_headline' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => TRUE,
    'layout_link_attribute' => FALSE,
    'layout_link_custom' => '',
  );
  $export['fancy_content|headline|inline_entity_form_preview'] = $ds_layout;

  return $export;
}
