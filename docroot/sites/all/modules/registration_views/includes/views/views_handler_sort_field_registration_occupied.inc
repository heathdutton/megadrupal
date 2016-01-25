<?php
/**
 * @file
 * Views handler for the registration occupîed sort.
 */
class views_handler_sort_field_registration_occupied extends views_handler_sort {

  function query() {
    $table_alias = $this->ensure_my_table();

    // Force group by
    $this->query->add_having_expression($this->options['group'], 'TRUE', array());

    $join = new views_join();
    $join->construct('registration', $this->table_alias, 'nid', 'nid', array(), 'LEFT');
    $alias = $this->query->ensure_table('registration_count', NULL, $join);

    // Add the field.
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $field = "IFNULL(SUM($alias.count), 0)";
    $this->field_alias = $this->query->add_field(FALSE, $field, 'occupied', $params);

    $this->query->add_orderby(NULL, NULL, $this->options['order'], $this->field_alias);
  }

}
