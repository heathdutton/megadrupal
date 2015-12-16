<?php

/**
 * Argument handler for payment transaction states.
 */
class pay_handler_argument_state extends views_handler_argument_string {

  /**
   * Override the behavior of title(). Get the title of the state.
   */
  function title() {
    return pay_transaction_state_name($this->argument);
  }
}
