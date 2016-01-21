<?php

/**
 * @file
 * views_handler_filter_support_client.inc
 */

class views_handler_filter_support_state extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = array();
    $result = db_query("SELECT sid, state FROM {support_states}");

    foreach ($result as $row) {
      $this->value_options[$row->state] = t(ucfirst($row->state));
    }
  }
}
