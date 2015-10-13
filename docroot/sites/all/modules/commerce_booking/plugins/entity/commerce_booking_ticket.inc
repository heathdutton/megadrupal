<?php
/**
 * @file
 * Definition of the commerce_booking_ticket plugin.
 */

$plugin = array(
  'handler' => 'PanelizerEntityCommerceBookingTicket',
  'uses page manager' => FALSE,
  'hooks' => array(
    'menu' => TRUE,
    'admin_paths' => TRUE,
    'permission' => TRUE,
    'panelizer_defaults' => TRUE,
    'form_alter' => TRUE,
    'views_data_alter' => TRUE,
  ),
);
