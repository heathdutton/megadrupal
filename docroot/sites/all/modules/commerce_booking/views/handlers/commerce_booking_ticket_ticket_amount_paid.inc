<?php
/**
 * @file
 * Contains a Views field handler to show the amount paid for a ticket
 */

class commerce_booking_ticket_ticket_amount_paid extends views_handler_field {

  /**
   * Overrides views_handler_field::construct().
   */
  function construct() {
    parent::construct();

    $this->additional_fields['ticket_id'] = 'ticket_id';
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $values->{$this->aliases['ticket_id']});
    $paid = $ticket_wrapper->paid->value();

    return commerce_currency_format($paid['amount'], $paid['currency_code']);
  }
}
