<?php

/**
 * @file
 * Defines Rules integration functions for the Reservation module.
 */

/**
 * Implements hook_rules_event_info().
 */
function reservation_rules_event_info() {
  return array(
    'reservation_validate' => array(
      'label' => t('Validating a reservation'),
      'group' => t('Reservation'),
      'variables' => array(
        'reservation' => array(
          'type' => 'reservation',
          'label' => t('Reservation'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function reservation_rules_condition_info() {
  return array(
    'reservation_is_unit_type' => array(
      'label' => t('Reservation is of unit type'),
      'group' => t('Reservation'),
      'parameter' => array(
        'reservation' => array(
          'type' => '*',
          'label' => t('Reservation'),
          'description' => t('The reservation to check.'),
        ),
        'type' => array(
          'type' => 'text',
          'label' => t('Type'),
          'description' => t('The unit type to check for.'),
          'options list' => 'reservation_unit_types',
          'restriction' => 'input',
        ),
      ),
    ),
    'reservation_time_available' => array(
      'label' => t('Reservation time available'),
      'group' => t('Reservation'),
      'parameter' => array(
        'reservation' => array(
          'type' => '*',
          'label' => t('Reservation'),
          'description' => t('The reservation to check.'),
        ),
      ),
    ),
    'reservation_user_allowed' => array(
      'label' => t('User allowed'),
      'group' => t('Reservation'),
      'parameter' => array(
        'reservation' => array(
          'type' => '*',
          'label' => t('Reservation'),
          'description' => t('The reservation to check'),
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user to check.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function reservation_rules_action_info() {
  return array(
    'reservation_set_validation_error' => array(
      'label' => t('Set a validation error'),
      'group' => t('Reservation'),
      'parameter' => array(
        'element' => array(
          'type' => '*',
          'label' => t('Form Element'),
          'description' => t('The form element to set the error on.'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t('The message to display to the user.'),
        ),
      ),
    ),
  );
}

/**
 * Rules condition to check if a reservation is of a unit type.
 */
function reservation_is_unit_type($reservation, $type) {
  $unit = reservation_unit_load($reservation->unit);

  return $unit->type == $type;
}

/**
 * Rules condition to check if a unit is available.
 */
function reservation_time_available($reservation) {
  $conflicts = array();
  $dates = $reservation->reservation_date[LANGUAGE_NONE];

  foreach ($dates as $date) {
    if (!reservation_available($reservation, $date, 'reservation_date')) {
      $conflicts[] = format_date($date['value'], 'short');
    }
  }

  if (empty($conflicts)) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Rules action to set a validation error on the reservation form.
 */
function reservation_set_validation_error($element, $message, $settings) {
  $field = _reservation_rules_extract_field($settings['element:select']);
  form_set_error($field, check_plain($message));
}

/**
 * Extracts a field name from a Rules selector.
 *
 * @param $selector
 *   The selector to extract the field name from.
 *
 * @return
 *   The field name.
 */
function _reservation_rules_extract_field($selector) {
  $pos = strrpos($selector, ':') + 1;
  $field = str_replace('-', '_', drupal_substr($selector, $pos));

  return $field;
}
