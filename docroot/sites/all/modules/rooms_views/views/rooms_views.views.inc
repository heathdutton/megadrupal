<?php

/**
 * @file
 * Providing extra functionality for Rooms via Views.
 */


/**
 * Implements hook_views_data().
 */
function rooms_views_views_data() {
  $data['rooms_units']['availability'] = array(
    'title' => t('Availability'),
    'filter' => array(
      'title' => t('Rooms Availability'),
      'help' => t('Filter Bookable units by their availability'),
      'handler' => 'rooms_views_handler_availability_filter',
    ),
  );

  // Price calculations only available when calculating price per night.
  if (variable_get('rooms_price_calculation', ROOMS_PER_NIGHT) == ROOMS_PER_NIGHT) {
    $data['rooms_units']['booking'] = array(
      'title' => t('Booking form'),
      'field' => array(
        'title' => t('Booking form'),
        'help' => t('Shows a booking form for a Bookable unit'),
        'handler' => 'rooms_views_handler_booking_field',
      ),
    );
  }

  return $data;
}
