<?php
/**
 * @file
 * Implementations of hooks in the commerce_booking group.
 */

/**
 * Implements hook_default_commerce_booking_ticket_type().
 *
 * Create a standard ticket type.
 */
function commerce_booking_default_commerce_booking_ticket_type() {
  $types['commerce_booking_ticket'] = new CommerceBookingTicketType(array(
    'type' => 'commerce_booking_ticket',
    'label' => t('Standard'),
  ));

  return $types;
}

/**
 * Implements hook_commerce_booking_ticket_status_info().
 */
function commerce_booking_commerce_booking_ticket_status_info() {
  $statuses = array();

  $statuses['pending'] = array(
    'name' => 'pending',
    'label' => t('Pending'),
    'description' => t('The deposit has not yet been paid on this ticket.'),
    'default' => TRUE,
    'confirm to' => 'deposit_paid',
  );

  $statuses['deposit_paid'] = array(
    'name' => 'deposit_paid',
    'label' => t('Deposit Paid'),
    'description' => t('The deposit has paid on this ticket.'),
  );

  $statuses['paid_in_full'] = array(
    'name' => 'paid_in_full',
    'label' => t('Paid in Full'),
    'description' => t('This ticket has been paid for in full.'),
  );

  $statuses['cancelled'] = array(
    'name' => 'cancelled',
    'label' => t('Cancelled'),
    'description' => t('This ticket has been cancelled.'),
  );

  return $statuses;
}

/**
 * Implements hook_commerce_booking_ticket_access().
 */
function commerce_booking_commerce_booking_ticket_access($op, $ticket, $account) {
  // If we haven't got a ticket or ticket type, we can't do anything.
  if (!$ticket) {
    return;
  }

  // Get the type of ticket and use the normal permissions checks.
  if (is_object($ticket)) {
    $permission = "{$op} a {$ticket->type} ticket";
  }
  else {
    $permission = "{$op} a {$ticket} ticket";
  }
  if (user_access($permission, $account)) {
    return TRUE;
  }

  // Check out booking manager permissions.
  if (is_object($ticket) && $op != 'create') {
    $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket);
    if ($account->uid == $ticket_wrapper->line_item->order->owner->getIdentifier()) {
      switch ($op) {
        case 'view':
          return TRUE;

        case 'edit':
        case 'cancel':
          if (strpos($ticket->status, 'pending') !== FALSE) {
            return TRUE;
          }
      }
    }
  }
}

/**
 * Implements hook_commerce_booking_ticket_process().
 */
function commerce_booking_commerce_booking_ticket_process(CommerceBookingTicket &$ticket) {
  // Update the status if necessary.
  if (in_array($ticket->status, array('deposit_paid', 'paid_in_full'))) {
    // Get the balance of the ticket.
    $balance = commerce_booking_ticket_balance($ticket);
    $ticket->status = $balance['amount'] <= 0 ? 'paid_in_full': 'deposit_paid';
  }
}
