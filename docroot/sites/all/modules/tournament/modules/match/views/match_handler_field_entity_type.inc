<?php

/**
 * @file
 * Views handler for Match module
 */

class match_handler_field_entity_type extends views_handler_field {
  function render($values) {
    $info = entity_get_info();
    $entity_type = $values->{$this->field_alias};
    return $info[$entity_type]['label'];
  }
}
