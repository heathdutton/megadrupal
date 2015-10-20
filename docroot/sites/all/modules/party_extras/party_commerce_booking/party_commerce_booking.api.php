<?php
/**
 * @file
 * Hooks provided by Party Commerce Booking.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to respond to newly acquired tickets.
 *
 * @param Party $party
 *   The party for the acquisition.
 * @param string $acquisition_method
 *   Indicates the type of acquisition:
 *     - update: The party was specifically set on the ticket.
 *     - create: The party was newly created.
 *     - acquire: The party was acquired via email address.
 * @param CommerceBookingTicket $ticket
 *   The ticket that has just been acquired.
 */
function hook_party_commerce_booking_party_acquisition($party, $acquisition_method, $ticket) {
  // No example.
}

/**
 * Allow modules to set values on a newly created ticket with a known party.
 *
 * @param CommerceBookingTicket $ticket
 *   The new ticket, not yet saved.
 * @param Party $party
 *   The party the ticket belongs to.
 * @param object $booking
 *   The commerce_order of type commerce_booking that the ticket will be
 *   attached to.
 */
function hook_party_commerce_booking_party_add_ticket_alter(&$ticket, $party, $booking) {
  // Copy the party label into a name field.
  if ($ticket->type == 'my_type') {
    $ticket->name[LANGAUGE_NONE][0]['value'] = $party->label;
  }
}

/**
 * @} End of "addtogroup hooks".
 */
