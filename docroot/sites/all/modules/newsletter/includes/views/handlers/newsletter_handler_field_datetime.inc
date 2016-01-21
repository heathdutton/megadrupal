<?php

/**
 * @file
 * Views handler for newsletter date SQL fields.
 */

/**
 * Format an SQL date value to a UNIX timestamp before rendering.
 */
class newsletter_handler_field_datetime extends views_handler_field_date {

  function render($values) {
    $values->{$this->field_alias} = strtotime($values->{$this->field_alias});
    // Render the date using the normal date handler
    return parent::render($values);
  }
}
