<?php

/**
 * @file
 * Definition of views_handler_filter_source.
 */

/**
 * Filter by source.
 *
 * @ingroup views_filter_handlers
 */
class social_content_views_handler_filter_source extends views_handler_filter_in_operator {

  function get_value_options() {

    if (isset($this->value_options)) {
      return;
    }

    static $classes = array();
    if (!$classes) {
      $classes = social_content_get_classes();
    }

    $this->value_options = $classes;
    foreach ($classes as $machine_name => $class) {
      $this->value_options[$machine_name] = ucfirst($machine_name);
    }
  }
}
