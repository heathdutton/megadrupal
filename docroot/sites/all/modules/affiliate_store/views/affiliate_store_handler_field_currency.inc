<?php

/**
 * @file
 * Views field handler for price currency.
 */

/**
 * Field handler for price currency.
 *
 * Convert a currency three characters code to its symbol representation.
 *
 * @ingroup views_field_handlers
 */
class affiliate_store_handler_field_currency extends views_handler_field {
  /**
   * Render a price currency into its symbol representation.
   */
  function render($values) {
    $code = $this->get_value($values);
    return theme('affiliate_store_currency', array('code' => $code));
  }
}
