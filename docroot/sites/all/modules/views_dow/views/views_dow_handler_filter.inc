<?php

/**
 * @file
 * Contains a Views filter handler to filter results by current day of week.
 */

class views_dow_handler_filter extends views_handler_filter {
  function admin_summary() {
    return t('Is today');
  }

  function can_expose() {
    return FALSE;
  }

  function query() {
    $today = date('N');
    $table = $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    $this->query->add_where($this->options['group'], $field, $today, "=");
  }
}
