<?php
/**
 * @file
 * Hooks provided by Commerce Booking.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define Commerce Booking Ticket statuses.
 *
 * Commerce Booking defines the following statuses by default:
 * - Pending: The ticket has been requested (added to a booking) but no deposit
 *   has been paid yet.
 * - Deposit Paid: The deposit has been paid on this ticket.
 * - Paid in Full: This ticket is confirmed and has been paid for in full.
 * - Cancelled: The ticket has been cancelled.
 *
 * @return array
 *   An array of status arrays keyed by name.
 */
function hook_commerce_booking_ticket_status_info() {
  $statuses = array();

  $statuses['pending'] = array(
    'name' => 'pending',
    'label' => t('Pending'),
    'description' => t('The deposit has not yet been paid on this ticket.'),
    'default' => TRUE,
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
 * Allow modules to alter Commerce Booking Ticket statuses.
 *
 * @param array $statuses
 *   An array of Status arrays keyed by name.
 *
 * @see hook_commerce_booking_ticket_status_info()
 */
function hook_commerce_bookng_ticket_status_info_alter(&$statuses) {
  // No example.
}

/**
 * Influence whether a booking can be created.
 *
 * This allows modules to deny a user from creating a booking. Examples would
 * be a module that links tickets to users and therefore wants to prevent
 * ticket holders booking again or a module that tracks capacity of an event.
 *
 * If any module returns FALSE, a booking will be denied.
 *
 * @param string $entity_type
 *   The entity type of the even entity.
 * @param Object $entity
 *   The event entity.
 * @param stdClass $account
 *   The account to prepare the booking for.
 *
 * @return NULL|FALSE
 *   Return FALSE to deny an order being created.
 *
 * @see commerce_booking_prepare_booking()
 */
function hook_commerce_booking_allow_booking($entity_type, $entity, $account) {
  // Check whether there is capacity for more bookings on this event.
  if (mymodule_event_is_full($entity_type, $entity)) {
    return FALSE;
  }
}

/**
 * Alter a booking when it is created, before saving.
 *
 * @param stdClass $booking
 *   The commerce_order of type commerce_booking.
 */
function hook_commerce_booking_order_alter(&$booking) {
  // No example.
}

/**
 * Indicate whether a ticket is locked.
 *
 * Locked tickets will not be processed. This allows you to preserve
 * information such as status, class and price.
 *
 * @param CommerceBookingTicket $ticket
 *   The ticket entity.
 * @param string $entity_type
 *   The type of event entity.
 * @param object $entity
 *   The event entity.
 *
 * @return bool|NULL
 *   Return TRUE to indicate the ticket is locked and should not be processed.
 */
function hook_commerce_booking_ticket_is_locked(CommerceBookingTicket $ticket, $entity_type, $entity) {
  // Unpublished node events should be locked.
  if ($entity_type == 'node' && !$entity->status) {
    return TRUE;
  }
}

/**
 * React to a ticket being confirmed.
 *
 * Implementations should not trigger a save on the ticket as this will be done
 * by the invoker.
 *
 * @param CommerceBookingTicket $ticket
 *   The ticket that has been confirmed.
 */
function hook_commerce_booking_ticket_confirm($ticket) {
  // Send an email to the site admin to inform them.
  $mail = variable_get('site_mail');
  if ($mail) {
    $params = array(
      'ticket_id' => $ticket->ticket_id,
    );
    drupal_mail('mymodule', 'ticket_confirm', $mail, LANGUAGE_NONE, $params);
  }
}

/**
 * Allow or deny access to a ticket.
 *
 * @param string $op
 *   The operation that is being performed.
 * @param CommerceBookingTicket $ticket
 *   The ticket the operation is being performed on, if any.
 * @param $account
 *   The user account that is requesting the operation.
 *
 * @return bool|NULL
 *   FALSE will deny access, regardless of other modules. TRUE will allow
 *   access providing no other modules return FALSE. NULL will not influence.
 *   If no modules return TRUE, access will be denied.
 */
function hook_commerce_booking_ticket_access($op, CommerceBookingTicket $ticket = NULL, $account = NULL) {
  // Always allow access to user 1.
  if ($account->uid == 1) {
    return TRUE;
  }
}

/**
 * Modify the price of a ticket.
 *
 * @param array $price_field
 *   A commerce price field item array.
 * @param CommerceBookingTicket $ticket
 *   The ticket entity.
 * @param array $options
 *   An array of options. Options can be set when processing a ticket to
 *   indicate specific actions required, normally by the module's own hooks.
 */
function hook_commerce_booking_ticket_price_alter(&$price_field, CommerceBookingTicket $ticket, $options) {
  // No example.
}

/**
 * Process a ticket.
 *
 * @param CommerceBookingTicket $ticket
 *   The ticket entity.
 * @param array $options
 *   An array of options. Options can be set when processing a ticket to
 *   indicate specific actions required, normally by the module's own hooks.
 */
function hook_commerce_booking_ticket_process(CommerceBookingTicket $ticket, $options) {
  // No example.
}

/**
 * Allow modules to add additional access checks to event subpage views.
 *
 * @param $view_name
 *   The machine name of the view in question.
 * @param $display_id
 *   The machine name of the display in question.
 * @param $entity_type
 *   The entity type we are working with.
 * @param $entity
 *   The entity we are working with.
 *
 * @return bool|NULL
 *   Return FALSE to deny access. Everything else is ignored.
 */
function hook_commerce_booking_views_event_subpage_access($view_name, $display_id, $entity_type, $entity) {
  // Only allow access to event subpages if you can edit the event.
  return entity_access('update', $entity_type, $entity);
}

/**
 * @} End of "addtogroup hooks".
 */
