<?php

/**
 * @file
 * Views handler for field that exposes serialized field instance settings.
 */

/**
 * @inheritdoc
 */
class architecture_field_instance_label_views_handler extends views_handler_field {

  /**
  * Render the preview settings for this nodetype.
  */
  function render($values) {
    $value = unserialize($this->get_value($values));
    return $this->sanitize_value($value['label']);
  }
}
