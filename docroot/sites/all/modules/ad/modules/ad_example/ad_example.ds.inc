<?php
/**
 * @file
 * ad_example.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function ad_example_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|ad_example|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'ad_example';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'post_date' => array(
      'weight' => '1',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
    'changed_date' => array(
      'weight' => '2',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
    'submitted_by' => array(
      'weight' => '3',
      'label' => 'inline',
      'format' => 'ds_time_ago',
    ),
  );
  $export['node|ad_example|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|ad_example|example_advertisement';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'ad_example';
  $ds_fieldsetting->view_mode = 'example_advertisement';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h2',
        'class' => '',
      ),
    ),
  );
  $export['node|ad_example|example_advertisement'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|ad_example|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'ad_example';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'ad_total_clicks' => array(
      'weight' => '5',
      'label' => 'inline',
      'format' => 'default',
    ),
    'ad_total_impressions' => array(
      'weight' => '6',
      'label' => 'inline',
      'format' => 'default',
    ),
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'post_date' => array(
      'weight' => '2',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
    'changed_date' => array(
      'weight' => '3',
      'label' => 'inline',
      'format' => 'ds_post_date_long',
    ),
    'submitted_by' => array(
      'weight' => '1',
      'label' => 'inline',
      'format' => 'ds_time_ago',
    ),
  );
  $export['node|ad_example|full'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function ad_example_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'ad_total_clicks';
  $ds_field->label = 'Total Clicks';
  $ds_field->field_type = 3;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array();
  $export['ad_total_clicks'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'ad_total_impressions';
  $ds_field->label = 'Total Impressions';
  $ds_field->field_type = 3;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = '';
  $ds_field->properties = array();
  $export['ad_total_impressions'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function ad_example_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|ad_example|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'ad_example';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'post_date',
        2 => 'changed_date',
        3 => 'submitted_by',
        4 => 'field_ad_size',
        5 => 'field_ad_image',
        6 => 'field_ad_link',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'post_date' => 'ds_content',
      'changed_date' => 'ds_content',
      'submitted_by' => 'ds_content',
      'field_ad_size' => 'ds_content',
      'field_ad_image' => 'ds_content',
      'field_ad_link' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|ad_example|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|ad_example|example_advertisement_display';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'ad_example';
  $ds_layout->view_mode = 'example_advertisement_display';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_ad_image',
      ),
    ),
    'fields' => array(
      'field_ad_image' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|ad_example|example_advertisement_display'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|ad_example|example_advertisement_preview';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'ad_example';
  $ds_layout->view_mode = 'example_advertisement_preview';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_ad_image',
      ),
    ),
    'fields' => array(
      'field_ad_image' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|ad_example|example_advertisement_preview'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|ad_example|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'ad_example';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'submitted_by',
        2 => 'post_date',
        3 => 'changed_date',
        4 => 'field_ad_size',
        5 => 'ad_total_clicks',
        6 => 'ad_total_impressions',
        7 => 'field_ad_image',
        8 => 'field_ad_link',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'submitted_by' => 'ds_content',
      'post_date' => 'ds_content',
      'changed_date' => 'ds_content',
      'field_ad_size' => 'ds_content',
      'ad_total_clicks' => 'ds_content',
      'ad_total_impressions' => 'ds_content',
      'field_ad_image' => 'ds_content',
      'field_ad_link' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|ad_example|full'] = $ds_layout;

  return $export;
}

/**
 * Implements hook_ds_view_modes_info().
 */
function ad_example_ds_view_modes_info() {
  $export = array();

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'example_advertisement_display';
  $ds_view_mode->label = 'Example Advertisement Display';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['example_advertisement_display'] = $ds_view_mode;

  $ds_view_mode = new stdClass();
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'example_advertisement_preview';
  $ds_view_mode->label = 'Example Advertisement Preview';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $export['example_advertisement_preview'] = $ds_view_mode;

  return $export;
}
