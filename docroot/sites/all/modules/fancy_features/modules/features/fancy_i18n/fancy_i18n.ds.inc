<?php
/**
 * @file
 * fancy_i18n.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function fancy_i18n_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'content_i18n|text_i18n|default';
  $ds_layout->entity_type = 'content_i18n';
  $ds_layout->bundle = 'text_i18n';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'fancy_grid_one';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_text_i18n',
      ),
    ),
    'fields' => array(
      'field_fancy_text_i18n' => 'one',
    ),
    'classes' => array(),
    'wrappers' => array(),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => TRUE,
    'layout_link_attribute' => FALSE,
    'layout_link_custom' => '',
    'layout_disable_css' => FALSE,
  );
  $export['content_i18n|text_i18n|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'fancy_section|responsive_i18n|default';
  $ds_layout->entity_type = 'fancy_section';
  $ds_layout->bundle = 'responsive_i18n';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'fancy_grid_one';
  $ds_layout->settings = array(
    'regions' => array(
      'one' => array(
        0 => 'field_fancy_rel_content_i18n',
      ),
    ),
    'fields' => array(
      'field_fancy_rel_content_i18n' => 'one',
    ),
    'classes' => array(
      'layout_class' => array(),
    ),
    'wrappers' => array(
      'one' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['fancy_section|responsive_i18n|default'] = $ds_layout;

  return $export;
}
