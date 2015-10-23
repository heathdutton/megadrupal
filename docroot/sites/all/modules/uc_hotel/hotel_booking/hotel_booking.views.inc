<?php
/*
 * @file hotel_booking.views.inc
 * Provides Views 2 Integration for hotel_booking_module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */
/**
 * Implementation of hook_views_handlers().
 */
function hotel_booking_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'hotel_booking') .'/views',
    ),
    'handlers' => array(
      'hotel_booking_handler_field_from_price' => array(
        'parent' => 'views_handler_field_numeric',
      )
    ),
  );
}


/**
 * Implementation of hook_views_data().
 */
function hotel_booking_views_data() {
  $data['hotel_booking_room_types']['table']['group'] = t('Hotel Booking');
  $data['hotel_booking_room_types']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
  );

  $data['hotel_booking_room_types']['capacity'] = array(
    'title' => t('Room capacity'),
    'help' => t('Capacity of the room.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric'
    )
  );

  $data['hotel_booking_room_types']['from_rate'] = array(
    'title' => t('From price for the room'),
    'help' => t("Gets the lowest base non-zero rate for any future
                date for this room type, includes rate modifiers only (no occupancy modifiers).
                This data is cached against the room type and requires an active cron."), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'hotel_booking_handler_field_from_price',
      'click sortable' => FALSE,
    )
  );

  return $data;
}
