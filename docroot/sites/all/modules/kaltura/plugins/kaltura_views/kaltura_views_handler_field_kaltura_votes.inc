<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_votes.
 */

/**
 * Votes field handler class.
 */
class kaltura_views_handler_field_kaltura_votes extends views_handler_field {
  function render($values) {
    $votes = $values->{$this->field_alias};
    return t('@votes counts', array('@votes' => $votes));
  }
}
