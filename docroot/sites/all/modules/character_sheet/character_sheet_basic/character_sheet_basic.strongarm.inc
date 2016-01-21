<?php
/**
 * @file
 * character_sheet_basic.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function character_sheet_basic_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_basic_attribute_cost';
  $strongarm->value = '3';
  $export['character_sheet_basic_attribute_cost'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_basic_skill_cost';
  $strongarm->value = '1';
  $export['character_sheet_basic_skill_cost'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_character_sheet_basic_playable';
  $strongarm->value = array(
    4 => '4',
  );
  $export['character_sheet_character_sheet_basic_playable'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_default_workflow';
  $strongarm->value = 'Character evaluation';
  $export['character_sheet_default_workflow'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_systems';
  $strongarm->value = array(
    'character_sheet_basic' => 'character_sheet_basic',
  );
  $export['character_sheet_systems'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_character_sheet';
  $strongarm->value = 'character_sheet_basic';
  $export['character_sheet_system_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_types';
  $strongarm->value = array(
    'character_sheet' => 'character_sheet_basic',
  );
  $export['character_sheet_types'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_character_sheet';
  $strongarm->value = '1';
  $export['comment_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_character_sheet';
  $strongarm->value = array();
  $export['menu_options_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_character_sheet'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_character_sheet';
  $strongarm->value = 1;
  $export['node_submitted_character_sheet'] = $strongarm;

  return $export;
}
