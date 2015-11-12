<?php
/**
 * @file
 * Entity classes for accommodation types.
 */

/**
 * The class used for commerce_booking_accom_type entities.
 */
class CommerceBookingAccomType extends Entity {

  /**
   * The accommodation type id.
   *
   * @var integer
   */
  public $accommodation_type_id;

  /**
   * The label of the accommodation type.
   *
   * @var string
   */
  public $label;

  /**
   * The category of the accommodation type.
   *
   * @var string
   */
  public $category;

  /**
   * Overrides Entity::defaultUri().
   */
  public function defaultUri() {
    $wrapper = entity_metadata_wrapper('commerce_booking_accom_type', $this);
    $uri = entity_uri($wrapper->commerce_booking_event->type(), $wrapper->commerce_booking_event->value());
    $uri['path'] .= '/accommodation/manage/' . $this->accommodation_type_id;
    return $uri;
  }

}
