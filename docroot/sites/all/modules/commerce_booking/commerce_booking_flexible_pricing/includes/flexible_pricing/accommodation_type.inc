<?php
/**
 * @file
 * Contains the flexible pricing handler for ticket class based prices.
 */

/**
 * Ticket flexible pricing handler based on booking window and ticket class.
 */
class CommerceFlexiblePricingHandlerAccommodationType extends CommerceFlexiblePricingHandlerBookingWindowBase {

  /**
   * The key for the Y-axis.
   */
  protected $y_key = 'accommodation_type_id';

  /**
   * Get a constructed price handler for an accommodation type.
   *
   * @param $accommodation_type
   *   The accommodation type we are working with.
   *
   * @return CommerceFlexiblePricingHandlerAccommodationType
   */
  public static function getHandler($accommodation_type) {
    $wrapper = entity_metadata_wrapper('commerce_booking_accom_type', $accommodation_type);

    $booking_field_name = commerce_booking_get_field_name($wrapper->commerce_booking_event->type(), $wrapper->commerce_booking_event->value());
    $booking_field_instance = field_info_instance($wrapper->commerce_booking_event->type(), $booking_field_name, $wrapper->commerce_booking_event->getBundle());
    $price_field_name = $booking_field_instance['settings']['accommodation_price_field'];

    $field = field_info_field($price_field_name);
    $instance = field_info_instance($wrapper->commerce_booking_event->type(), $price_field_name, $wrapper->commerce_booking_event->getBundle());

    return new CommerceFlexiblePricingHandlerAccommodationType($field, $instance);
  }

  /**
   * Figure out the booking window (X-axis).
   *
   * @param $line_item
   *   The accommodation type line item we want a booking window for.
   * @param array $options
   *   Optionally provide additional options for the price calculation. Possible
   *   keys include:
   *     - date: The date to use for calculating the current booking window.
   *       Defaults to REQUEST_TIME.
   *
   * @return int
   *   The booking window id.
   */
  public function getXValue($line_item, $options = array()) {
    $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

    // Check to see if we have an override.
    if ($window = $wrapper->cbt_booking_window->value()) {
      return $window['window_id'];
    }

    // Otherwise, use the normal process.
    return parent::getXValue($line_item, $options);
  }

  /**
   * Return the possible options for the y-axis.
   *
   * @param object $event
   *   The event entity.
   *
   * @return array
   *   An array of labels keyed by identifier.
   */
  protected function getYOptions($event) {
    $options = array();
    foreach ($this->getAccommodationTypes($event) as $accommodation_type) {
      $options[$accommodation_type->accommodation_type_id] = $accommodation_type->label;
    }
    return $options;
  }

  /**
   * Figure out the accommodation type (Y-axis).
   *
   * @param $line_item
   *   The accommodation type line item we want a booking window for.
   * @param array $options
   *   Optionally provide additional options for the price calculation. Possible
   *   keys include:
   *     - refresh_class: Whether to refresh the class cache. Defaults to FALSE.
   *
   * @return int
   *   An accommodation type id.
   */
  public function getYValue($line_item, $options = array()) {
    $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    return $wrapper->cba_accommodation_type->getIdentifier();
  }

  /**
   * {@inheritdoc}
   */
  protected function getPriceEntity($entity) {
    $wrapper = entity_metadata_wrapper('commerce_line_item', $entity);
    return $wrapper->cba_accommodation_type->commerce_booking_event->value();
  }

  /**
   * Calculate the accommodation type price from the booking window.
   *
   * @param $line_item
   *   The line item we are calculating a price for.
   * @param int $date
   *   Optionally provide a date to calculate the price at. Defaults to now.
   *
   * @return array
   *   A commerce price field array.
   */
  public function calculatePrice($line_item, $date = NULL) {
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
   * Return the possible values for the y-axis.
   *
   * @param object $event
   *   The event entity.
   *
   * @return CommerceBookingAccomType[]
   *   An array of labels keyed by identifier.
   */
  protected function getAccommodationTypes($event) {
    $wrapper = entity_metadata_wrapper($this->instance['entity_type'], $event);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_booking_accom_type');
    $query->fieldCondition('commerce_booking_event', 'entity_type', $this->instance['entity_type']);
    $query->fieldCondition('commerce_booking_event', 'entity_id', $wrapper->getIdentifier());
    $query->propertyOrderBy('label');

    $results = $query->execute();
    if (!empty($results['commerce_booking_accom_type'])) {
      return entity_load('commerce_booking_accom_type', array_keys($results['commerce_booking_accom_type']));
    }
    else {
      return array();
    }
  }

}
