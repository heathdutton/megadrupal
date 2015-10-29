<?php

/**
 * @file
 * A field hander for payment forms.
 */

class pay_handler_field_transaction_state extends views_handler_field {
  function render(&$values) {
    return pay_transaction_state_name($values->{$this->field_alias});
  }
}
