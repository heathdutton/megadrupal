<?php
/**
 * @file
 * white_wolf_character_sheets.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function white_wolf_character_sheets_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_changeling_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_default_workflow';
  $strongarm->value = 'Character evaluation';
  $export['character_sheet_default_workflow'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_hunter_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_mage_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_systems';
  $strongarm->value = array(
    'white_wolf_character_sheets' => 'white_wolf_character_sheets',
  );
  $export['character_sheet_systems'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_changeling_character_sheet';
  $strongarm->value = 'white_wolf_character_sheets';
  $export['character_sheet_system_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_hunter_character_sheet';
  $strongarm->value = 'white_wolf_character_sheets';
  $export['character_sheet_system_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_mage_character_sheet';
  $strongarm->value = 'white_wolf_character_sheets';
  $export['character_sheet_system_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_vampire_character_sheet';
  $strongarm->value = 'white_wolf_character_sheets';
  $export['character_sheet_system_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_system_werewolf_character_sheet';
  $strongarm->value = 'white_wolf_character_sheets';
  $export['character_sheet_system_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_types';
  $strongarm->value = array(
    'changeling_character_sheet' => 'white_wolf_character_sheets',
    'hunter_character_sheet' => 'white_wolf_character_sheets',
    'mage_character_sheet' => 'white_wolf_character_sheets',
    'vampire_character_sheet' => 'white_wolf_character_sheets',
    'werewolf_character_sheet' => 'white_wolf_character_sheets',
  );
  $export['character_sheet_types'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_vampire_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_werewolf_character_sheet';
  $strongarm->value = 1;
  $export['character_sheet_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'character_sheet_white_wolf_character_sheets_playable';
  $strongarm->value = array(
    4 => '4',
  );
  $export['character_sheet_white_wolf_character_sheets_playable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_changeling_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_hunter_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_mage_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_vampire_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_werewolf_character_sheet';
  $strongarm->value = 0;
  $export['comment_anonymous_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_changeling_character_sheet';
  $strongarm->value = '0';
  $export['comment_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_changeling_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_hunter_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_mage_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_vampire_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_werewolf_character_sheet';
  $strongarm->value = 1;
  $export['comment_default_mode_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_changeling_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_hunter_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_mage_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_vampire_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_werewolf_character_sheet';
  $strongarm->value = '50';
  $export['comment_default_per_page_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_changeling_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_hunter_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_mage_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_vampire_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_werewolf_character_sheet';
  $strongarm->value = 1;
  $export['comment_form_location_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_hunter_character_sheet';
  $strongarm->value = '0';
  $export['comment_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_mage_character_sheet';
  $strongarm->value = '0';
  $export['comment_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_changeling_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_hunter_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_mage_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_vampire_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_werewolf_character_sheet';
  $strongarm->value = '1';
  $export['comment_preview_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_changeling_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_hunter_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_mage_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_vampire_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_werewolf_character_sheet';
  $strongarm->value = 1;
  $export['comment_subject_field_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_vampire_character_sheet';
  $strongarm->value = '0';
  $export['comment_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_werewolf_character_sheet';
  $strongarm->value = '0';
  $export['comment_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_changeling_character_sheet';
  $strongarm->value = array();
  $export['menu_options_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_hunter_character_sheet';
  $strongarm->value = array();
  $export['menu_options_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_mage_character_sheet';
  $strongarm->value = array();
  $export['menu_options_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_vampire_character_sheet';
  $strongarm->value = array();
  $export['menu_options_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_werewolf_character_sheet';
  $strongarm->value = array();
  $export['menu_options_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_changeling_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_hunter_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_mage_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_vampire_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_werewolf_character_sheet';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_changeling_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_hunter_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_mage_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_vampire_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_werewolf_character_sheet';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_changeling_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_hunter_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_mage_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_vampire_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_werewolf_character_sheet';
  $strongarm->value = '1';
  $export['node_preview_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_changeling_character_sheet';
  $strongarm->value = 0;
  $export['node_submitted_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_hunter_character_sheet';
  $strongarm->value = 1;
  $export['node_submitted_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_mage_character_sheet';
  $strongarm->value = 0;
  $export['node_submitted_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_vampire_character_sheet';
  $strongarm->value = 0;
  $export['node_submitted_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_werewolf_character_sheet';
  $strongarm->value = 0;
  $export['node_submitted_werewolf_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workflow_changeling_character_sheet';
  $strongarm->value = array(
    0 => 'node',
  );
  $export['workflow_changeling_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workflow_hunter_character_sheet';
  $strongarm->value = array(
    0 => 'node',
  );
  $export['workflow_hunter_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workflow_mage_character_sheet';
  $strongarm->value = array(
    0 => 'node',
  );
  $export['workflow_mage_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workflow_vampire_character_sheet';
  $strongarm->value = array(
    0 => 'node',
  );
  $export['workflow_vampire_character_sheet'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workflow_werewolf_character_sheet';
  $strongarm->value = array(
    0 => 'node',
  );
  $export['workflow_werewolf_character_sheet'] = $strongarm;

  return $export;
}
