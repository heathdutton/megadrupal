<?php
/**
 * @file
 * openacademy_news.ds.inc
 */

/**
 * Implements hook_ds_layout_settings_info().
 */
function openacademy_news_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|openacademy_news|featured';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'openacademy_news';
  $ds_layout->view_mode = 'featured';
  $ds_layout->layout = 'phelan';
  $ds_layout->settings = array(
    'css_id' => '',
    'class' => 'node-featured',
    'sticky' => 0,
    'layout' => 'phelan',
    'hide_sidebars' => 0,
    'pipeline' => 'standard',
    'hide_empty_regions' => FALSE,
    'ds_panels' => TRUE,
    'did' => '3',
    'theme' => 'phelan',
  );
  $export['node|openacademy_news|featured'] = $ds_layout;

  return $export;
}
