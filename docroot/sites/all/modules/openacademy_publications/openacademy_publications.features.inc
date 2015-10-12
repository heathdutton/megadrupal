<?php
/**
 * @file
 * openacademy_publications.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function openacademy_publications_ctools_plugin_api($module = NULL, $api = NULL) {
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
function openacademy_publications_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function openacademy_publications_node_info() {
  $items = array(
    'openacademy_publication' => array(
      'name' => t('Publication'),
      'base' => 'node_content',
      'description' => t('An important publication to post to your site.
'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
