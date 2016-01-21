<?php

/**
 * @file
 * Availability calendar booking formlet translatable variables.
 */

/**
 * Implements hook_variable_info().
 */
function availability_calendar_booking_formlet_variable_info() {
  // Form labels
  $variables['availability_calendar_booking_formlet_date'] = array(
    'type' => 'string',
    'title' => t('Date field for the %option option', array('%option' => t('Only allow single day bookings'))),
    'description' => t('Label to use for the single date when you enabled the %option option.',
      array('%option' => t('Only allow single day bookings'))),
    'default' => t('Date'),
  );
  $variables['availability_calendar_booking_formlet_date_1'] = array(
    'type' => 'string',
    'title' => t('"Book now" form %1stOr2nd date field label', array('%1stOr2nd' => t('first'))),
    'default' => t('Arrival date'),
  );
  $variables['availability_calendar_booking_formlet_date_2'] = array(
    'type' => 'string',
    'title' => t('"Book now" form %1stOr2nd date field label', array('%1stOr2nd' => t('second'))),
    'default' => t('Departure date'),
  );
  $variables['availability_calendar_booking_formlet_duration'] = array(
    'type' => 'string',
    'title' => t('"Book now" form <em>duration</em> field label'),
    'default' => t('Duration'),
  );
  $variables['availability_calendar_booking_formlet_clear_date_text'] = array(
    'type' => 'string',
    'title' => t('"Book now" form clear single date button text'),
    'description' => t('"Book now" form clear date button text when you enabled the %option option.',
      array('%option' => t('Only allow single day bookings'))),
    'default' => t('Clear selected date'),
  );
  $variables['availability_calendar_booking_formlet_clear_date_title'] = array(
    'type' => 'string',
    'title' => t('"Book now" form clear single date button title'),
    'description' => t('"Book now" form clear date button title when you enabled the %option option.',
      array('%option' => t('Only allow single day bookings'))),
    'default' => t(''),
  );
  $variables['availability_calendar_booking_formlet_clear_date_1_text'] = array(
    'type' => 'string',
    'title' => t('"Book now" form clear first date button text'),
    'default' => t('Clear selected arrival date'),
  );
  $variables['availability_calendar_booking_formlet_clear_date_1_title'] = array(
    'type' => 'string',
    'title' => t('"Book now" form clear first date button title'),
    'default' => t(''),
  );
  $variables['availability_calendar_booking_formlet_clear_date_2_text'] = array(
    'type' => 'string',
    'title' => t('"Book now" form clear selected dates button text'),
    'default' => t('Clear selected dates'),
  );
  $variables['availability_calendar_booking_formlet_clear_date_2_title'] = array(
    'type' => 'string',
    'title' => t('"Book now" form clear selected dates button title'),
    'default' => t(''),
  );
  $variables['availability_calendar_booking_formlet_submit_text'] = array(
    'type' => 'string',
    'title' => t('"Book now" button text'),
    'default' => t('Book this resource ...'),
  );
  $variables['availability_calendar_booking_formlet_submit_title'] = array(
    'type' => 'string',
    'title' => t('"Book now" button title'),
    'default' => t(''),
  );

  foreach ($variables as &$variable) {
    $variable['group'] = 'availability_calendar';
  }

  return $variables;
}
