<?php

/**
 * @file
 * Implementation of date restrictions hooks.
 */

/**
 * Implements hook_date_restrictions_allowed_values_info().
 */
function work_calendar_date_restrictions_allowed_values_info() {
  return array(
    'work_calendar' => array(
      'title' => t('Work calendar'),
      'settings' => array(
        'work_calendar_id' => NULL,
        'reversed' => false,
      ),
    ),
  );
}

/**
 * Implements allowed_values_settings_form callback.
 */
function work_calendar_allowed_values_settings_form($type, $default_value, $context) {
  switch ($type) {
    case 'work_calendar':
      $element = array(
        '#theme_wrappers' => array('container'),
        '#attributes' => array('class' => array('container-inline')),
      );
      $element['work_calendar_id'] = work_calendar_form_element(TRUE, $default_value['work_calendar_id']);
      $element['reversed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Reverse: use closed days as available dates.'),
        '#default_value' => $default_value['reversed'],
      );
      break;
  }

  return $element;
}

/**
 * Implements allowed_values_valid_date callback.
 */
function work_calendar_allowed_values_valid_date($date, $type, $settings, $element, $form_state, $form) {
  switch ($type) {
    case 'work_calendar':
      $function = $settings['reversed'] ? 'work_calendar_day_is_closed' : 'work_calendar_day_is_open';
      $valid = $function($settings['work_calendar_id'], $date->format('Y'), $date->format('m'), $date->format('d'));
      break;
  }

  return $valid;
}

/**
 * Implements allowed_values_remitter_pack callback.
 */
function work_calendar_allowed_values_remitter_pack($type, $element, $form_state, $context) {
  switch ($type) {
    case 'work_calendar':
      $instance = $element['#instance'];
      return $instance['entity_type'] . ':' . $instance['bundle'] . ':' . $instance['field_id'];
      break;
  }
}

/**
 * Implements allowed_values_remitter_unpack callback.
 */
function work_calendar_allowed_values_remitter_unpack($type, $remitter) {
  switch ($type) {
    case 'work_calendar':
      list($entity_type, $bundle, $field_id) = explode(':', $remitter);
      $field = field_info_field_by_id($field_id);
      $instance = field_info_instance($entity_type, $field['field_name'], $bundle);
      $unpacked = $instance['settings']['restrictions']['allowed_values'][$type];
      break;
  }

  return $unpacked;
}

/**
 * Implements allowed_values_access_callback callback.
 */
function work_calendar_allowed_values_access_callback($type, $remitter) {
  switch ($type) {
    case 'work_calendar':
      $entity = entity_load_single('work_calendar', $remitter['work_calendar_id']);
      return entity_access('view', 'work_calendar', $entity);
      break;
  }
}

/**
 * Implements allowed_values_page_callback callback.
 */
function work_calendar_allowed_values_page_callback($type, $remitter, $year, $month) {
  switch ($type) {
    case 'work_calendar':
      $work_calendar = $remitter['work_calendar_id'];
      break;
  }
  $function = $remitter['reversed'] ? 'work_calendar_get_closed_days_in_month' : 'work_calendar_get_open_days_in_month';
  return $function($work_calendar, $year, $month);
}

