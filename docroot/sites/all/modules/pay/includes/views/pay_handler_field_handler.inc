<?php

/**
 * @file
 * A field hander for payment forms.
 */

class pay_handler_field_handler extends views_handler_field {
  function render(&$values) {
    $info = pay_handlers($this->definition['pay_type']);

    // Load the payment form and use the 'nice' label from hook_pay_handlers.
    $key = $values->{$this->definition['pay_key']};
    if ($pay = pay_load_object($this->definition['pay_type'], $key)) {
      return $info[$pay->handler()]['title'];
    }
  }
}
