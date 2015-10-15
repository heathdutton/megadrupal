<?php

/**
 * @file
 * Plugin to provide an relationship handler for booking from ticket.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Booking from Ticket'),
  'keyword' => 'commerce_booking_ticket',
  'description' => t('Adds the Booking entity from a Commerce Booking Ticket.'),
  'required context' => new ctools_context_required(t('Ticket'), 'commerce_booking_ticket'),
  'context' => 'commerce_booking_from_commerce_booking_ticket_context',
  'edit form' => 'commerce_booking_from_commerce_booking_ticket_form',
  'defaults' => array(),
);

/**
 * Return a new context based on an existing context.
 */
function commerce_booking_from_commerce_booking_ticket_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('entity:commerce_order', NULL);
  }

  $ticket = entity_metadata_wrapper('commerce_booking_ticket', $context->data);
  try {
    $booking = $ticket->line_item->order->value();
  } catch (Exception $e) {
    return FALSE;
  }

  return ctools_context_create('entity:commerce_order', $booking);
}

/**
 * Settings form for the relationship.
 */
function commerce_booking_from_commerce_booking_ticket_form($form, &$form_state) {
  return $form;
}
