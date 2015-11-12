<?php
/**
 * @file
 * Defines the CommerceBookingTicket class.
 */

/**
 * The class used for commerce_booking_ticket entities.
 */
class CommerceBookingTicket extends Entity {

  /**
   * The ticket id.
   *
   * @var integer
   */
  public $ticket_id;

  /**
   * The name of the ticket type.
   *
   * @var string
   */
  public $type;

  /**
   * The status of the ticket.
   *
   * @var string
   *
   * @see commerce_booking_ticket_status_info.
   */
  public $status;

  /**
   * Overrides Entity::construct().
   */
  public function __construct($values = array()) {
    // Set a default value on the status column if none is set.
    if (empty($this->status) && empty($values['status'])) {
      $status = 'pending';
      foreach (commerce_booking_ticket_status_info() as $settings) {
        if (!empty($settings['default'])) {
          $status = $settings['name'];
        }
      }

      $values['status'] = $status;
    }

    parent::__construct($values, 'commerce_booking_ticket');
  }

  /**
   * Overides Entity::defaultUri().
   */
  public function defaultUri() {
    return array(
      'path' => 'ticket/' . $this->ticket_id,
      'options' => array(''),
    );
  }

  /**
   * Get a human readable label for the entity.
   *
   * @todo: implement well.
   */
  public function defaultLabel() {
    if (!empty($this->label)) {
      return $this->label;
    }

    $entity_type = commerce_booking_get_event_entity_type($this);
    $entity = commerce_booking_get_event_entity($this);
    $label = entity_label($entity_type, $entity);
    return t('Ticket for @event', array('@event' => $label));
  }

  /**
   * Confirm the ticket, if necessary.
   *
   * @param boolean $store
   *   Whether we need to trigger a save to store the change. Note, a save is
   *   only triggered if the status is updated.
   *
   * @return CommerceBookingTicket
   *   The called object.
   *
   * @see hook_commerce_booking_ticket_confirm()
   */
  public function confirm($store = TRUE) {
    // If we need to update the status, do so.
    $statuses = commerce_booking_ticket_status_info();
    if (isset($statuses[$this->status]['confirm to'])) {
      $original_status = $this->status;
      $this->status = $statuses[$this->status]['confirm to'];
      $this->confirmed = REQUEST_TIME;

      // Allow module to react to the ticket confirmation.
      module_invoke_all('commerce_booking_ticket_confirm', $this, $original_status);

      if ($store) {
        $this->save();
      }
    }

    return $this;
  }
}
