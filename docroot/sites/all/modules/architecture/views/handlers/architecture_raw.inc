<?php

/**
 * @file
 * Views handler override for field that exposes raw data.
 */


/**
 * @inheritdoc
 */
class architecture_raw_views_handler extends views_handler_field {

  /**
  * Render the raw values.
  */
  function render($values) {
    return ($this->get_value($values));
  }
}
