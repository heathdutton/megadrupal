<?php
/**
 * @file
 * Hooks provided by Commerce Booking Flexible Pricing.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define Commerce Booking Ticket statuses.
 *
 * @param array $conditions
 *   An array of conditions for rules_config_load_multiple().
 * @param array $forced_classes
 *   An array of forced class names grouped by:
 *   - always: These classes are always available.
 *   - never: These classes are never available.
 * @param $event_wrapper
 *   An entity metadata wrapper for the event.
 *
 * @see commerce_booking_flexible_pricing_get_ticket_classes()
 */
function hook_commerce_booking_flexible_pricing_ticket_class_conditions_alter($conditions, $forced_classes, $event_wrapper) {
  // No example
}

/**
 * @} End of "addtogroup hooks".
 */
