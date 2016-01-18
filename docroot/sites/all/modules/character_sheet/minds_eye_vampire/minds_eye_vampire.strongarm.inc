<?php
/**
 * @file
 * minds_eye_vampire.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function minds_eye_vampire_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'additional_settings__active_tab_me_v_character_sheet';
  $strongarm->value = 'edit-character-sheet';
  $export['additional_settings__active_tab_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_default_workflow';
  $strongarm->value = 'Character evaluation';
  $export['character_sheet_default_workflow'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_me_v_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_minds_eye_vampire_playable';
  $strongarm->value = array(
    4 => '4',
  );
  $export['character_sheet_minds_eye_vampire_playable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_me_v_character_sheet';
  $strongarm->value = 'minds_eye_vampire';
  $export['character_sheet_system_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_me_v_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_me_v_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_me_v_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_me_v_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_me_v_character_sheet';
  $strongarm->value = '0';
  $export['comment_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_me_v_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_me_v_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__me_v_character_sheet';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_me_v_character_sheet';
  $strongarm->value = array();
  $export['menu_options_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_me_v_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_me_v_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_me_v_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_me_v_character_sheet';
  $strongarm->value = 0;
  $export['node_submitted_me_v_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workflow_me_v_character_sheet';
  $strongarm->value = array(
    0 => 'node',
  );
  $export['workflow_me_v_character_sheet'] = $strongarm;

  return $export;
}
