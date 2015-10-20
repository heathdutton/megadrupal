<?php
/**
 * @file
 * openacademy_people.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function openacademy_people_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function openacademy_people_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function openacademy_people_node_info() {
  $items = array(
    'openacademy_person' => array(
      'name' => t('Person'),
      'base' => 'node_content',
      'description' => t('An important person to post to your site.
'),
      'has_title' => '1',
      'title_label' => t('Name of Person'),
      'help' => '',
    ),
  );
  return $items;
}
