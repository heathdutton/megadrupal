<?php

/**
 * Custom handler for reviews.
 */
class reviews_handler_field_review extends views_handler_field {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $review = unserialize($values->reviews_review);
    if (!is_null($review)) {
      return check_markup($review['value'], $review['format']);
    }
    else {
      return '';
    }
  }

}
