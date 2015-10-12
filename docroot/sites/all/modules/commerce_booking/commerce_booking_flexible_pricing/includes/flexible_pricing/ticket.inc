<?php
/**
 * @file
 * Contains the flexible pricing handler for ticket class based prices.
 */

/**
 * Ticket flexible pricing handler based on booking window and ticket class.
 */
class CommerceFlexiblePricingHandlerTicket extends CommerceFlexiblePricingHandlerBookingWindowBase {

  /**
   * The key for the Y-axis.
   */
  protected $y_key = 'class';

  /**
   * Get a constructed price handler for a ticket.
   *
   * @param $ticket
   *   The ticket we are working with.
   *
   * @return CommerceFlexiblePricingHandlerTicket
   */
  public static function getHandler($ticket) {
    $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket);

    $booking_field_name = commerce_booking_get_field_name($ticket_wrapper->commerce_booking_event->type(), $ticket_wrapper->commerce_booking_event->value());
    $booking_field_instance = field_info_instance($ticket_wrapper->commerce_booking_event->type(), $booking_field_name, $ticket_wrapper->commerce_booking_event->getBundle());
    $price_field_name = $booking_field_instance['settings']['price_field'];

    $field = field_info_field($price_field_name);
    $instance = field_info_instance($ticket_wrapper->commerce_booking_event->type(), $price_field_name, $ticket_wrapper->commerce_booking_event->getBundle());

    return new CommerceFlexiblePricingHandlerTicket($field, $instance);
  }

  /**
   * Figure out the booking window (X-axis).
   *
   * @param $ticket
   *   The ticket we want the booking window for.
   * @param array $options
   *   Optionally provide additional options for the price calculation. Possible
   *   keys include:
   *     - date: The date to use for calculating the current booking window.
   *       Defaults to REQUEST_TIME.
   *
   * @return int
   *   The booking window id.
   */
  public function getXValue($ticket, $options = array()) {
    $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket);

    // Check to see if we have an override.
    if ($window = $ticket_wrapper->cbt_booking_window->value()) {
      return $window['window_id'];
    }

    // Otherwise, use the normal process.
    return parent::getXValue($ticket, $options);
  }

  /**
   * {@inheritdoc}
   */
  protected function getYOptions($event) {
    $options = array();
    foreach ($this->getTicketClasses($event) as $class) {
      $options[$class->name] = $class->label;
    }
    return $options;
  }

  /**
   * Figure out the ticket class (Y-axis).
   *
   * @param $ticket
   *   The ticket we want the ticket class for.
   * @param array $options
   *   Optionally provide additional options for the price calculation. Possible
   *   keys include:
   *     - refresh_class: Whether to refresh the class cache. Defaults to FALSE.
   *
   * @return string
   *   A ticket class name.
   */
  public function getYValue($ticket, $options = array()) {
    $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket);

    // First look for a cached version if we are not refreshing.
    if (empty($options['refresh_class'])) {
      if ($cached_class = $ticket_wrapper->cbt_class_cache->value()) {
        return $cached_class->name;
      }
    }

    // See if there is a hard override.
    if ($class = $ticket_wrapper->commerce_booking_ticket_class->value()) {
      $ticket->cbt_class_cache[LANGUAGE_NONE][0]['target_id'] = $class->id;
      return $class->name;
    }

    // Otherwise, loop over the possible classes finding the first match.
    foreach (commerce_booking_flexible_pricing_get_ticket_classes($ticket_wrapper->commerce_booking_event) as $class) {
      // Check the rule evaluates true!
      if (rules_invoke_component($class->name, $ticket)) {
        // Store the class and return.
        $ticket->cbt_class_cache[LANGUAGE_NONE][0]['target_id'] = $class->id;
        return $class->name;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function getPriceEntity($entity) {
    $wrapper = entity_metadata_wrapper('commerce_booking_ticket', $entity);
    return $wrapper->commerce_booking_event->value();
  }

  /**
   * Calculate the price for a ticket.
   *
   * @param $ticket
   *   The ticket we want the price for.
   * @param array $options
   *   Optionally provide additional options for the price calculation. Possible
   *   keys include:
   *     - date: The date to use for calculating the current booking window.
   *       Defaults to REQUEST_TIME.
   *     - refresh_class: Whether to refresh the class cache. Defaults to FALSE.
   *
   * @return array
   *   A commerce price field array or NULL if there is no valid value.
   */
  public function calculatePrice($ticket, $options = array()) {
    $price = parent::calculatePrice($ticket, $options);

    // If there is no price, return zero price.
    if ($price === NULL) {
      $price = array(
        'amount' => 0,
        'currency_code' => commerce_default_currency(),
      );
    }

    return $price;
  }

  /**
   * Get the ticket classes enabled on this event.
   *
   * @param object $event
   *   The event entity.
   * @param bool $sort
   *   Whether to sort the ticket classes. Defaults to TRUE.
   *
   * @return array
   *   An array of ticket classes.
   */
  protected function getTicketClasses($event, $sort = TRUE) {
    $wrapper = entity_metadata_wrapper($this->instance['entity_type'], $event);
    $classes = commerce_booking_flexible_pricing_get_ticket_classes($wrapper);

    if ($sort) {
      usort($classes, array($this, 'sortTicketClasses'));
    }

    return $classes;
  }

  /**
   * Comparison callback for sorting ticket classes.
   */
  protected function sortTicketClasses($a, $b) {
    // Get our weights, defaulting to 0.
    $a_weight = isset($a->weight) ? $a->weight: 0;
    $b_weight = isset($b->weight) ? $b->weight: 0;

    // Equal weights fall back to name comparison.
    if ($a_weight == $b_weight) {
      return strnatcasecmp($a->label, $b->label);
    }
    // Otherwise, return based off weight.
    else {
      return $a_weight > $b_weight ? 1: -1;
    }
  }

}
