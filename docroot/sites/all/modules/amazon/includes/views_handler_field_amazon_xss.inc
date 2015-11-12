<?php

/**
 * @defgroup views_field_handlers Views' field handlers
 * @{
 * Handlers to tell Views how to build and display fields.
 *
 */

/**
 * A handler to run a field through simple XSS filtering
 */
class views_handler_field_amazon_xss extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    return filter_xss_admin($value);
  }
}

/**
 * @}
 */
