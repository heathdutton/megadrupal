<?php

/**
 * Implements hook_views_data()
 */
function bookit_cart_views_data() {
  $data = array();

  // Booking button.
  $data['bookit_item']['booking_button'] = array(
    'title' => t('Booking Button'),
    'help' => t('Displays a booking button for a bookable item.'),
    'field' => array(
      'real field' => 'item_id',
      'handler' => 'bookit_item_handler_field_booking_button',
    ),
  );

  // Booking quantity.
  $data['bookit_item']['booking_quantity'] = array(
    'title' => t('Booking Quantity'),
    'help' => t('Displays a booking quantity form element.'),
    'field' => array(
      'real field' => 'item_id',
      'handler' => 'bookit_item_handler_field_booking_quantity',
    ),
  );

  return $data;
}
