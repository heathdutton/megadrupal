<?php

/**
 * @file
 * ooyala_handler_field_length.inc
 *
 * Provides a handler for displaying video length column.
 */
class ooyala_handler_field_length extends views_handler_field_node {
  function render($values) {
    $values = clone($values); // Prevent affecting the original.
    $length = $values->{$this->field_alias};
    return theme('ooyala_video_length', array('length' => $length));
  }
}
