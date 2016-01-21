<?php

/**
 * A handler to display the current server timestamp in a view. 
 * @ingroup views_field_handlers
 */
class views_now_handler_field extends views_handler_field {
  function query() {
    // do nothing
  }
  /**
   * Implements views_handler_field#render().
   */
  function render($values) {
    $value = time();
    return $value;
  }
}

