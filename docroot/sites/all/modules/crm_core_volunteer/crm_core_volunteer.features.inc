<?php
/**
 * @file
 * crm_core_volunteer.features.inc
 */

/**
 * Implements hook_default_crm_core_activity_type().
 */
function crm_core_volunteer_default_crm_core_activity_type() {
  $items = array();
  $items['cmcv_volunteer_commitment'] = entity_import('crm_core_activity_type', '{
    "type" : "cmcv_volunteer_commitment",
    "label" : "Volunteer Commitment",
    "weight" : 0,
    "activity_string" : "volunteered for the opportunity",
    "description" : "",
    "rdf_mapping" : []
  }');
  $items['cmcv_volunteer_interests'] = entity_import('crm_core_activity_type', '{
    "type" : "cmcv_volunteer_interests",
    "label" : "Volunteer Interests",
    "weight" : 0,
    "activity_string" : "",
    "description" : "",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function crm_core_volunteer_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function crm_core_volunteer_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function crm_core_volunteer_node_info() {
  $items = array(
    'cmcv_volunteer' => array(
      'name' => t('Volunteer Opportunity'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
