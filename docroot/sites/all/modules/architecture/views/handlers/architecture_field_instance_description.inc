<?php

/**
 * @file
 * Views handler for field.
 */

/**
 * @inheritdoc
 */
class architecture_field_instance_description_views_handler extends views_handler_field {

  /**
   * Render the required settings for this nodetype.
   */
  function render($values) {
    $data = unserialize(($this->get_value($values)));
    return $data['description'];
  }
}
