<?php

/**
 * @file
 * Filter by city type
 */
class views_handler_filter_city extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('City');
      $cities = fortissimmo_get_property_cities();
      foreach ($cities as $value => $name) {
        $options[$value] = t($name);
      }
      $this->value_options = $options;
    }
  }
}
