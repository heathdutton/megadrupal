<?php
/**
 * @file
 * Views handler for the registration available sort.
 */
class views_handler_sort_field_registration_available extends views_handler_sort {

  function query() {
    $this->ensure_my_table();

    // Force group by
    $this->query->add_having_expression($this->options['group'], 'TRUE', array());

    $join = new views_join();
    $join->construct('registration', $this->table_alias, 'nid', 'nid', array(), 'LEFT');
    $alias = $this->query->ensure_table('registration_count', NULL, $join);

    // Add the field.
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $field = "(IF(IFNULL($this->table_alias.capacity, 0) = 0, 9999, $this->table_alias.capacity) - IFNULL(SUM($alias.count), 0))";
    $this->field_alias = $this->query->add_field(FALSE, $field, 'available', $params);

    $this->query->add_orderby(NULL, NULL, $this->options['order'], $this->field_alias);
  }

}
