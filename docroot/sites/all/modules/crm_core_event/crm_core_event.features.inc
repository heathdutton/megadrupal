<?php
/**
 * @file
 * crm_core_event.features.inc
 */

/**
 * Implements hook_default_crm_core_activity_type().
 */
function crm_core_event_default_crm_core_activity_type() {
  $items = array();
  $items['cmcev_event_registration'] = entity_import('crm_core_activity_type', '{
    "type" : "cmcev_event_registration",
    "label" : "Event Registration",
    "weight" : 0,
    "activity_string" : "registered for the event",
    "description" : "Stores details about registration for events in the system.",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function crm_core_event_ctools_plugin_api($module = NULL, $api = NULL) {
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
function crm_core_event_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function crm_core_event_node_info() {
  $items = array(
    'cmcev_event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event that will display on the online calendar.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
