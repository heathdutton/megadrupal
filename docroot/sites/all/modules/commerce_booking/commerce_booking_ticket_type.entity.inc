<?php
/**
 * @file
 * Defines the CommerceBookingTicketType class.
 */

/**
 * Use a separate class for commerce_booking_ticket types so we can specify
 * some defaults modules may alter.
 */
class CommerceBookingTicketType extends Entity {

  /**
   * Machine name of the Ticket type.
   *
   * @var string
   */
  public $type;

  /**
   * Human readable name of the Ticket type.
   *
   * @var string
   */
  public $label;

  /**
   * Overrides Entity::__construct().
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_booking_ticket_type');
  }

  /**
   * Returns whether the entity is locket.
   *
   * Project types provided in code are automatically treated as locked, as well
   * as any fixed project type.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}
