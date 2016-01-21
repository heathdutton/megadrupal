<?php
/**
 * @file
 * user_priority_ranking_example.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function user_priority_ranking_example_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'flag_limit_wants';
  $strongarm->value = 1;
  $export['flag_limit_wants'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'flag_limit_wants_value';
  $strongarm->value = '3';
  $export['flag_limit_wants_value'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_toys_user_priority_ranking';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_toys_user_priority_ranking'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_toys_user_priority_ranking';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_toys_user_priority_ranking'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_toys_user_priority_ranking';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_toys_user_priority_ranking'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_toys_user_priority_ranking';
  $strongarm->value = '1';
  $export['node_preview_toys_user_priority_ranking'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_toys_user_priority_ranking';
  $strongarm->value = 0;
  $export['node_submitted_toys_user_priority_ranking'] = $strongarm;

  return $export;
}
