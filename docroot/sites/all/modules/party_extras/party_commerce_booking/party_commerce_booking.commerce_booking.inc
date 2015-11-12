<?php
/**
 * @file
 * Implementations of hooks in the commerce_booking group.
 */

/**
 * Implements hook_commerce_booking_allow_booking().
 *
 * Prevent a user who's party already has a ticket from booking onto an event.
 */
function party_commerce_booking_commerce_booking_allow_booking($entity_type, $entity, $account) {
  // Get hold of our party for the account.
  $party = party_user_get_party($account);

  if ($party) {
    // Extract our entity id and look for tickets.
    list($entity_id) = entity_extract_ids($entity_type, $entity);
    $tickets = party_commerce_booking_party_tickets($party, $entity_type, $entity_id);

    // If we got any tickets back, deny the order creation.
    if (count($tickets)) {
      drupal_set_message(t('You already have a ticket for %event.', array('%event' => entity_label($entity_type, $entity))), 'error');
      return FALSE;
    }
  }
}

/**
 * Implements hook_commerce_booking_order_alter().
 *
 * Get the party for a given order.
 */
function party_commerce_booking_commerce_booking_order_alter($booking) {
  if (!empty($booking->uid) && empty($booking->booking_manager)) {
    $booking_wrapper = entity_metadata_wrapper('commerce_order', $booking);

    $party = party_user_get_party($booking_wrapper->owner->value());
    $booking_wrapper->booking_manager = entity_metadata_wrapper('party', $party);
    $booking_wrapper->save();
  }
}

/**
 * Implements hook_commerce_booking_ticket_access().
 */
function party_commerce_booking_commerce_booking_ticket_access($op, $ticket, $account) {
  // If this is not a ticket or is an unknown operation we do nothing.
  if (!in_array($op, array('edit', 'delete', 'view', 'cancel'))) {
    return;
  }

  $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket);

  // Get the party from the user.
  $party = party_user_get_party($account);
  if (!$party) {
    return;
  }

  // Check ticket holder permissions.
  if ($party->pid == $ticket_wrapper->party->getIdentifier()) {
    if (user_access("{$op} own party {$ticket->type} ticket", $account)) {
      // Edit and cancel can only happen on pending tickets.
      if ($op == 'edit' || $op == 'cancel') {
        if (strpos($ticket->status, 'pending') !== FALSE) {
          return TRUE;
        }
      }
      else {
        return TRUE;
      }
    }
  }

  // Check booking manager permissions.
  if ($ticket_wrapper->line_item->value() && $ticket_wrapper->line_item->order->value()) {
    if ($party->pid == $ticket_wrapper->line_item->order->booking_manager->getIdentifier()) {
      switch ($op) {
        case 'view':
          return TRUE;

        case 'edit':
        case 'cancel':
          return strpos($ticket->status, 'pending') !== FALSE ? TRUE: NULL;
      }
    }
  }
}
