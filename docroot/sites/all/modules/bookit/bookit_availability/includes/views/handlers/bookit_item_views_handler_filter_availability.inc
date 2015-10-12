<?php

class bookit_item_views_handler_filter_availability extends views_handler_filter_group_by_numeric {
  function query() {
    $this->ensure_my_table();

    if ($booking_date = bookit_calendar_date_by_view($this->view)) {
      $join = new views_join();
      $def = array(
        'type' => 'INNER',
        'table' => 'bookit_availability',
        'field' => 'item_id',
        'left_table' => $this->table_alias,
        'left_field' => 'item_id',
      );

      if (isset($booking_date['min'])) {
        $def['extra'] = array(
          array(
            'field' => 'date',
            'operator' => '>=',
            'value' => $booking_date['min'],
          ),
          array(
            'field' => 'date',
            'operator' => '<',
            'value' => $booking_date['max'],
          ),
        );
      }
      else {
        $def['extra'] = array(
          array(
            'field' => 'date',
            'operator' => '=',
            'value' => $booking_date['value'],
          ),
        );
      }

      $join->definition = $def;
      $join->construct();
      $join->adjusted = TRUE;
      $this->table_alias = $this->query->ensure_table('bookit_availability', $this->relationship, $join);
      $this->field_alias = $this->query->add_field($this->table_alias, 'availability', NULL, array('function' => 'min'));

      $info = $this->operators();
      if (!empty($info[$this->operator]['method'])) {
        $this->{$info[$this->operator]['method']}($this->field_alias);
      }
    }
  }
}
