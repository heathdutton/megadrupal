<?php
/**
 * @file
 * Views handler for the registration capacity field.
 */
class views_handler_field_registration_capacity extends views_handler_field_numeric {
  function query() {
    $this->ensure_my_table();

    // Add the field.
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $field = "IF($this->table_alias.capacity = 0, 9999, $this->table_alias.capacity)";
    $this->field_alias = $this->query->add_field(FALSE, $field, 'capacity', $params);

    $this->add_additional_fields();
  }
}