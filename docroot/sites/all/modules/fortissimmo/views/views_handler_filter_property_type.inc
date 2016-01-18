<?php

/**
 * @file
 * Filter by property type
 */
class views_handler_filter_property_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Property type');
      $types = fortissimmo_get_types();
      foreach ($types as $value => $name) {
        $options[$value] = t($name);
      }
      $this->value_options = $options;
    }
  }
}
