<?php

/**
 * @file
 * Definition of event_data_store_views_handler_filter_event_name.
 */

/**
 * Basic textfield filter to handle string filtering commands
 * including equality, like, not like, etc.
 *
 * @ingroup views_filter_handlers
 */
class event_data_store_views_handler_filter_event_name extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Event Data Stores to query');
      $triggers = event_data_store_event_load_all();
      $options = array();
      foreach ($triggers as $type => $info) {
        $options[$type] = t($info->name);
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}
