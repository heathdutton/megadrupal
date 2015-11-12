<?php

/**
 * Implements hook_rules_action_info().
 */
function party_commerce_booking_rules_action_info() {
  return array(
    'party_commerce_booking_action_get_booking_from_party' => array(
      'label' => t('Get booking from party'),
      'parameter' => array(
        'party' => array(
          'label' => t('Party'),
          'type' => 'party',
          'wrapped' => FALSE,
        ),
        'event' => array(
          'label' => t('Event (entity)'),
          'type' => 'entity',
          'wrapped' => TRUE,
        ),
        'create' => array(
          'label' => t('Create if none'),
          'type' => 'boolean',
        ),
      ),
      'provides' => array(
        'order' => array(
          'label' => t('Order'),
          'type' => 'commerce_order',
        ),
      ),
      'group' => t('Party Commerce Booking'),
    ),
  );
}

/**
 * Rules action for returning an order for a party for an event.
 *
 * @param $party
 *   The party we want an order for.
 * @param $entity_wrapper
 *   The event the order is for.
 * @param $create
 *   Optionally create an order if there isn't already one.
 */
function party_commerce_booking_action_get_booking_from_party($party, $entity_wrapper, $create = FALSE) {
  // Attempt to find a user for this party.
  $controller = $party->getDataSetController('user');
  $account = $controller->getEntity();
  if (!$account) {
    $account = drupal_anonymous_user();
  }

  // Find any tickets this party has for this event.
  $tickets = party_commerce_booking_party_tickets($party, $entity_wrapper->type(), $entity_wrapper->getIdentifier());

  // If we have a ticket, return the order.
  if (!empty($tickets)) {
    // Find our line item as that has an order id.
    if ($line_item = commerce_booking_get_line_item(reset($tickets))) {
      $order = entity_load_single('commerce_order', $line_item->order_id);
    }
  }
  else {
    // Otherwise we need to create a ticket and possibly a booking. First attempt
    // to find a booking if there is an account.
    $order = $account->uid ? commerce_booking_get_booking($account, $entity_wrapper->type(), $entity_wrapper->value()): FALSE;

    if ($order) {
      // Create the initial ticket for this booking.
      $ticket = commerce_booking_ticket_create($order);
      entity_save('commerce_booking_ticket', $ticket);

      // We have a booking, so let's add a ticket for this party.
      $line_item = commerce_line_item_new('commerce_booking_ticket', $order->order_id);
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $line_item_wrapper->commerce_booking_ticket[] = $ticket;
      commerce_booking_line_item_ticket_process($line_item_wrapper->value(), $order);
      $line_item_wrapper->save();

      // Add the line item to the booking.
      $booking_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $booking_wrapper->commerce_line_items[] = $line_item;
      $booking_wrapper->save();
    }
    else {
      // There is no booking, so we need to create one. This also adds a ticket
      // for our party.
      $order = commerce_booking_prepare_booking($entity_wrapper->type(), $entity_wrapper->value(), $account);
    }
  }

  return array('order' => $order);
}
