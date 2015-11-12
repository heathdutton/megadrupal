<?php
/**
 * @file
 * waypoints_ui_demo.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function waypoints_ui_demo_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_waypoints_ui_demo_page';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_waypoints_ui_demo_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_waypoints_ui_demo_page';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_waypoints_ui_demo_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_waypoints_ui_demo_page';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_waypoints_ui_demo_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_waypoints_ui_demo_page';
  $strongarm->value = '1';
  $export['node_preview_waypoints_ui_demo_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_waypoints_ui_demo_page';
  $strongarm->value = 0;
  $export['node_submitted_waypoints_ui_demo_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'save_continue_waypoints_ui_demo_page';
  $strongarm->value = 'Save and add fields';
  $export['save_continue_waypoints_ui_demo_page'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'waypoints_ui_waypoints';
  $strongarm->value = array(
    0 => array(
      'attach_element' => '.field-name-field-first-text',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '40',
      'remove' => 'Remove',
    ),
    1 => array(
      'attach_element' => '.field-name-field-second-text',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '40',
      'remove' => 'Remove',
    ),
    2 => array(
      'attach_element' => '.field-name-field-third-text',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '40',
      'remove' => 'Remove',
    ),
    3 => array(
      'attach_element' => '.field-name-field-fourth-text',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '40',
      'remove' => 'Remove',
    ),
    4 => array(
      'attach_element' => '.field-name-field-first-image',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '50',
      'remove' => 'Remove',
    ),
    5 => array(
      'attach_element' => '.field-name-field-second-image',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '50',
      'remove' => 'Remove',
    ),
    6 => array(
      'attach_element' => '.field-name-field-third-image',
      'toggle_class' => 'waypoints-ui-visible',
      'same_element' => 1,
      'toggle_element' => '',
      'toggle_offset' => '50',
      'remove' => 'Remove',
    ),
  );
  $export['waypoints_ui_waypoints'] = $strongarm;

  return $export;
}
