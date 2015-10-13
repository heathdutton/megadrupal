<?php

class bookit_item_views_handler_filter_price extends views_handler_filter_group_by_numeric {
  function query() {
    $this->ensure_my_table();

    if ($booking_date = bookit_calendar_date_by_view($this->view)) {
      // Build the join subquery.
      $subquery = db_select('bookit_price', 'p');
      $subquery->fields('p', array('item_id'));
      $subquery->addExpression('SUM(p.amount)', 'amount');
      $subquery->groupBy('p.item_id');

      if (isset($booking_date['min'])) {
        $subquery->condition('p.date', $booking_date['min'], '>=');
        $subquery->condition('p.date', $booking_date['max'], '<');
      }
      else {
        $subquery->condition('p.date', $booking_date['value']);
      }

      $join = new views_join();
      $join->definition = array(
        'type' => 'INNER',
        'table' => '',
        'table formula' => $subquery,
        'field' => 'item_id',
        'left_table' => $this->table_alias,
        'left_field' => 'item_id',
      );
      $join->construct();
      $join->adjusted = TRUE;
      $this->table_alias = $this->query->ensure_table('bookit_price', $this->relationship, $join);
      $this->field_alias = $this->query->add_field($this->table_alias, 'amount', NULL, array('function' => 'min'));

      $info = $this->operators();
      if (!empty($info[$this->operator]['method'])) {
        $this->{$info[$this->operator]['method']}($this->field_alias);
      }
    }
  }
}
