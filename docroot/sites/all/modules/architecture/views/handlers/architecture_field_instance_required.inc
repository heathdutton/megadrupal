<?php

/**
 * @file
 * Views handler for field that exposes "required" setting.
 */

/**
 * @inheritdoc
 */
class architecture_field_instance_required_views_handler extends views_handler_field {

  /**
  * Render the required settings for this nodetype.
  */
  function render($values) {
    $field_info = field_info_field($this->get_value($values));
    $field_instance_info = field_info_instance('node', $values->field_config_field_name, $values->field_config_instance_bundle);
    return($field_instance_info['required'] ? 'Yes':'No');
  }
}
