<?php

class codes_handler_filter_field_codes extends views_handler_filter_in_operator {
  function get_value_options() {
    $field = field_info_field($this->definition['field_name']);
    $this->value_options = codes_options_list($field);
  }
}
