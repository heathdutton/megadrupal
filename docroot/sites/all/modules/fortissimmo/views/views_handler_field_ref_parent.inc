<?php

/**
 * @file
 * Views handler for ref_parent field
 */

class views_handler_field_ref_parent extends views_handler_field {

  function render($values) {
    $parentid = $values->{$this->field_alias};
    if ($parentid) {
      $title = fortissimmo_property_get_title($parentid);
      if ($title) {
        return fortissimmo_generate_link($title, $parentid);
      }
      else {
        return NULL;
      }
    }
    return NULL;
  }
}