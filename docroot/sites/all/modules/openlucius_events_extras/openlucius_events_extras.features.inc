<?php
/**
 * @file
 * openlucius_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function openlucius_events_extras_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function openlucius_events_extras_flag_default_flags() {
  $flags = array();
  // Exported flag: "I will attend".
  $flags['attending_event'] = array(
    'entity_type' => 'flagging',
    'title' => 'I will attend',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'I will attend',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'I will not attend',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => 'You are not allowed to unflag this.',
    'link_type' => 'normal',
    'weight' => -7,
    'show_in_links' => array(
      'ical' => 'ical',
      'diff_standard' => 0,
      'token' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'module' => 'openlucius_events_extras',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "I will not attend".
  $flags['not_attending_event'] = array(
    'entity_type' => 'flagging',
    'title' => 'I will not attend',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'I will not attend',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'I will attend',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'weight' => -6,
    'show_in_links' => array(
      'ical' => 'ical',
      'diff_standard' => 0,
      'token' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'flag_confirmation' => 'Why will you not attend?',
    'unflag_confirmation' => '',
    'module' => 'openlucius_events_extras',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}
