<?php
/**
 * @file
 * Contains a Views field handler to show the balance of a ticket
 */

class commerce_booking_ticket_ticket_balance extends views_handler_field {

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
    $balance = $ticket_wrapper->balance->value();

    return commerce_currency_format($balance['amount'], $balance['currency_code']);
  }
}
