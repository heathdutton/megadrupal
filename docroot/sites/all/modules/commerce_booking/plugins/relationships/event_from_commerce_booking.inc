<?php

/**
 * @file
 * Plugin to provide an relationship handler for event from commerce_order.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Event entity from Booking'),
  'keyword' => 'event_from_booking',
  'description' => t('Adds a the Event entity from a Commerce Booking order.'),
  'required context' => new ctools_context_required(t('Booking'), 'commerce_order'),
  'context' => 'commerce_booking_event_from_commerce_booking_context',
  'edit form' => 'commerce_booking_event_from_commerce_booking_settings_form',
  'defaults' => array(),
);

/**
 * Return a new context based on an existing context.
 */
function commerce_booking_event_from_commerce_booking_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('entity:node', NULL);
  }

  $booking_wrapper = entity_metadata_wrapper('commerce_order', $context->data);

  if (!isset($booking_wrapper->commerce_booking_event)) {
    return FALSE;
  }

  $event_wrapper = $booking_wrapper->commerce_booking_event;

  return ctools_context_create('entity:' . $event_wrapper->type(), $event_wrapper->value());
}

/**
 * Settings form for the relationship.
 */
function commerce_booking_event_from_commerce_booking_settings_form($form, &$form_state) {
  return $form;
}
