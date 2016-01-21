<?php

/**
 * @file
 * Availability calendar translatable variables.
 */

/**
 * Implements hook_variable_group_info().
 */
function availability_calendar_variable_group_info() {
  $groups['availability_calendar'] = array(
    'title' => t('Availability Calendar'),
    'description' => t('Customizable texts for the Availability Calendar UI'),
    'access' => 'translate interface',
    'path' => array('admin/config/content/availability-calendar'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function availability_calendar_variable_info() {
  // Show in Colorbox formatter/widget
  $variables['availability_calendar_colorbox_link_text'] = array(
    'type' => 'string',
    'title' => t('Text for the link to Colorbox'),
    'description' => t('Text for the button that displays the calendar in Colorbox.'),
    'default' => t('Click to see the calendar'),
  );
  $variables['availability_calendar_colorbox_link_title'] = array(
    'type' => 'string',
    'title' => t('Title attribute for the link to Colorbox'),
    'description' => t('Title attribute for the button that displays the calendar in Colorbox.'),
    'default' => '',
  );

  // Viewport formatter/widget
  $variables['availability_calendar_viewport_button_prev_1_text'] = array(
    'type' => 'string',
    'title' => t('Previous month button text'),
    'description' => t('Text for the button that moves the viewport 1 month back.'),
    'default' => t('Previous month'),
  );
  $variables['availability_calendar_viewport_button_prev_1_title'] = array(
    'type' => 'string',
    'title' => t('Previous month button title'),
    'description' => t('Title attribute for the button that moves the viewport 1 month back.'),
    'default' => t(''),
  );
  $variables['availability_calendar_viewport_button_prev_n_text'] = array(
    'type' => 'string',
    'title' => t('Previous {n} months button text'),
    'description' => t('Text for the button that moves the viewport {n} months back.'),
    'default' => t('Previous @count months'),
  );
  $variables['availability_calendar_viewport_button_prev_n_title'] = array(
    'type' => 'string',
    'title' => t('Previous {n} months button title'),
    'description' => t('Title attribute for the button that moves the viewport {n} months back.'),
    'default' => t(''),
  );
  $variables['availability_calendar_viewport_button_prev_unknown_text'] = array(
    'type' => 'string',
    'title' => t('Previous month(s) button text'),
    'description' => t('Text for the button that moves the viewport 1 or more months back.'),
    'default' => t('Previous month(s)'),
  );
  $variables['availability_calendar_viewport_button_prev_unknown_title'] = array(
    'type' => 'string',
    'title' => t('Previous month(s) button title'),
    'description' => t('Title attribute for the button that moves the viewport 1 or more months back.'),
    'default' => t('Previous month(s)'),
  );
  $variables['availability_calendar_viewport_button_next_1_text'] = array(
    'type' => 'string',
    'title' => t('Next month button text'),
    'description' => t('Text for the button that moves the viewport 1 month forward.'),
    'default' => t('Next month'),
  );
  $variables['availability_calendar_viewport_button_next_1_title'] = array(
    'type' => 'string',
    'title' => t('Next month button title'),
    'description' => t('Title attribute for the button that moves the viewport 1 month forward.'),
    'default' => t('Next month'),
  );
  $variables['availability_calendar_viewport_button_next_n_text'] = array(
    'type' => 'string',
    'title' => t('Next {n} months button text'),
    'description' => t('Text for the button that moves the viewport {n} months forward.'),
    'default' => t('Next @count months'),
  );
  $variables['availability_calendar_viewport_button_next_n_title'] = array(
    'type' => 'string',
    'title' => t('Next {n} months button title'),
    'description' => t('Title attribute for the button that moves the viewport {n} months forward.'),
    'default' => t('Next @count months'),
  );
  $variables['availability_calendar_viewport_button_next_unknown_text'] = array(
    'type' => 'string',
    'title' => t('Next month(s) button text'),
    'description' => t('Text for the button that moves the viewport 1 or more months forward.'),
    'default' => t('Next month(s)'),
  );
  $variables['availability_calendar_viewport_button_next_unknown_title'] = array(
    'type' => 'string',
    'title' => t('Next month(s) button title'),
    'description' => t('Title attribute for the button that moves the viewport 1 or more months forward.'),
    'default' => t('Next month(s)'),
  );

  // Views operand labels
  $variables['availability_calendar_views_op_at_from'] = array(
    'type' => 'string',
    'title' => t('Date for the %op operator', array('%op' => t('At (date)'))),
    'description' => t('Label to use for the date when exposing an availability search with an %op operator.',
      array('%op' => t('At (date)'))),
    'default' => t('At'),
  );
  $variables['availability_calendar_views_op_from_to_from'] = array(
    'type' => 'string',
    'title' => t('@1stOr2nd date for the %op operator', array('@1stOr2nd' => t('First'), '%op' => t('From begin up to and including end'))),
    'description' => t('Label to use for the @1stOr2nd date when exposing an availability search with an %op operator.',
      array('@1stOr2nd' => t('first'), '%op' => t('From begin up to and including end'))),
    'default' => t('Start date'),
  );
  $variables['availability_calendar_views_op_from_to_to'] = array(
    'type' => 'string',
    'title' => t('@1stOr2nd date for the %op operator', array('@1stOr2nd' => t('Second'), '%op' => t('From begin up to and including end'))),
    'description' => t('Label to use for the @1stOr2nd date when exposing an availability search with an %op operator.',
      array('@1stOr2nd' => t('second'), '%op' => t('From begin up to and including end'))),
    'default' => t('End date'),
  );
  $variables['availability_calendar_views_op_from_to1_from'] = array(
    'type' => 'string',
    'title' => t('@1stOr2nd date for the %op operator', array('@1stOr2nd' => t('First'), '%op' => t('From arrival to departure'))),
    'description' => t('Label to use for the @1stOr2nd date when exposing an availability search with an %op operator.',
      array('@1stOr2nd' => t('first'), '%op' => t('From arrival to departure'))),
    'default' => t('Arrival date'),
  );
  $variables['availability_calendar_views_op_from_to1_to1'] = array(
    'type' => 'string',
    'title' => t('@1stOr2nd date for the %op operator', array('@1stOr2nd' => t('Second'), '%op' => t('From arrival to departure'))),
    'description' => t('Label to use for the @1stOr2nd date when exposing an availability search with an %op operator.',
      array('@1stOr2nd' => t('second'), '%op' => t('From arrival to departure'))),
    'default' => t('Departure date'),
  );
  $variables['availability_calendar_views_op_from_duration_from'] = array(
    'type' => 'string',
    'title' => t('@1stOr2nd date for the %op operator', array('@1stOr2nd' => t('First'), '%op' => t('From begin during duration'))),
    'description' => t('Label to use for the @1stOr2nd date when exposing an availability search with an %op operator.',
      array('@1stOr2nd' => t('first'), '%op' => t('From begin during duration'))),
    'default' => t('Start date'),
  );
  $variables['availability_calendar_views_op_from_duration_duration'] = array(
    'type' => 'string',
    'title' => t('Duration for the %op operator', array('@1stOr2nd' => t('Second'), '%op' => t('From begin during duration'))),
    'description' => t('Label to use for the duration when exposing an availability search with an %op operator.',
      array('@1stOr2nd' => t('second'), '%op' => t('From begin during duration'))),
    'default' => t('Duration'),
  );

  foreach ($variables as &$variable) {
    $variable['group'] = 'availability_calendar';
  }

  return $variables;
}
