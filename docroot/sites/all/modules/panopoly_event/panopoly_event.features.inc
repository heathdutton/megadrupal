<?php
/**
 * @file
 * panopoly_event.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function panopoly_event_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "panelizer" && $api == "panelizer") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function panopoly_event_node_info() {
  $items = array(
    'panopoly_event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event is a way to create a date/time registration page. '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_registration_type().
 */
function panopoly_event_default_registration_type() {
  $items = array();
  $items['free_event'] = entity_import('registration_type', '{
    "name" : "free_event",
    "label" : "Free Event",
    "locked" : "0",
    "weight" : "0",
    "data" : null
  }');
  return $items;
}
