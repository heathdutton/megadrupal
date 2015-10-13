<?php

/**
 * @file
 * Contains the flagged content filter handler.
 */

/**
 * Handler to filter for content that has not been flagged.
 *
 * @ingroup views
 */

class track_field_changes_handler_filter_field_info extends views_handler_filter_in_operator {
  function get_value_options() {
    $node_types = variable_get('track_field_changes_node_types', array());
    foreach ($node_types as $key => $node_type) {
      if ($node_type) {
        $selected_fields = array_merge((array)$selected_fields, (array)track_field_changes_get_selected_field($node_type));
      }
    }
    $this->value_options = $selected_fields;
  }
}
