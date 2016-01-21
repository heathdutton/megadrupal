<?php
/**
 * @file
 * Implements the field handler class.
 */

class yashare_handler_field_yashare extends views_handler_field_entity {
  public function render($values) {
    $output = '';
    if ($entity = $this->get_value($values)) {
      $output = _yashare_widget_view($entity, $this->entity_type);
    }
    return $output;
  }
}
