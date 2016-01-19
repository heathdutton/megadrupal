<?php

/**
 * @file
 * Views handler for filter Domain Type.
 */

class views_handler_filter_domain_type extends views_handler_filter_in_operator {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Domain types');
      $options = _domain_types_names();
      asort($options);
      $this->value_options = $options;
    }
  }

}
