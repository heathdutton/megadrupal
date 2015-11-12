<?php

class bookit_option_views_handler_filter_price extends views_handler_filter_group_by_numeric {
  function query() {
    $this->ensure_my_table();
    $this->query->add_field($this->table_alias, $this->real_field);

    if ($date = bookit_calendar_date_by_view($this->view)) {
      // Build the join subquery.
      $subquery = db_select('bookit_option_price', 'p');
      $subquery->fields('p', array('item_id', 'option_id'));
      $subquery->addExpression('SUM(p.amount)', 'amount');
      $subquery->groupBy('p.item_id');
      $subquery->groupBy('p.option_id');

      if (isset($date['min'])) {
        $subquery->condition('p.date', $date['min'], '>=');
        $subquery->condition('p.date', $date['max'], '<');
      }
      else {
        $subquery->condition('p.date', $date['value']);
      }

      $join = new views_join();
      $join->definition = array(
        'type' => 'LEFT',
        'table' => '',
        'table formula' => $subquery,
        'field' => 'item_id',
        'left_table' => $this->table_alias,
        'left_field' => 'entity_id',
        'extra' => "bookit_option_price.option_id = {$this->table_alias}.{$this->real_field}",
      );
      $join->construct();
      $join->adjusted = TRUE;
      $this->table_alias = $this->query->ensure_table('bookit_option_price', $this->relationship, $join);
      $this->field_alias = $this->query->add_field($this->table_alias, 'amount', NULL, array('function' => 'min'));

      $info = $this->operators();
      if (!empty($info[$this->operator]['method'])) {
        $this->{$info[$this->operator]['method']}($this->field_alias);
      }
    }
  }
}
