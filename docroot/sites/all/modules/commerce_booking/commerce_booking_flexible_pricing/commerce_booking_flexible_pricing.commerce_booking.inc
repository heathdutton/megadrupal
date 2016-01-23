<?php
/**
 * @file
 * Implementations of hooks in the commerce_booking group.
 */

/**
 * Implements hook_commerce_booking_ticket_price_alter().
 */
function commerce_booking_flexible_pricing_commerce_booking_ticket_price_alter(&$price_field, $ticket, $options) {
  $options = isset($options['commerce_booking_flexible_pricing']) ? $options['commerce_booking_flexible_pricing']: array();
  $options += array('refresh_class' => TRUE);
  $handler = CommerceFlexiblePricingHandlerTicket::getHandler($ticket);
  if ($price = $handler->calculatePrice($ticket, $options)) {
    $price_field = array(
      LANGUAGE_NONE => array($price),
    );
  }
}

/**
 * Implements hook_commerce_booking_flexible_pricing_fields_handlers().
 */
function commerce_booking_flexible_pricing_commerce_booking_flexible_pricing_fields_handlers() {
  return array(
    'ticket' => array(
      'handler' => 'CommerceFlexiblePricingHandlerTicket',
      'label' => t('Ticket: Booking window/ticket class'),
    ),
    'accommodation_type' => array(
      'handler' => 'CommerceFlexiblePricingHandlerAccommodationType',
      'label' => t('Accommodation type: Booking window'),
    ),
  );
}
