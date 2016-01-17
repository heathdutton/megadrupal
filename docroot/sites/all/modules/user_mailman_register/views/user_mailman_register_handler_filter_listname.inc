<?php
class user_mailman_register_handler_filter_listname extends views_handler_filter_many_to_one {
  function get_value_options() {
    $this->value_options = array();
    $result = db_query("SELECT name FROM {mailman_lists} ORDER BY name")
      ->fetchAll();
    foreach ($result as $list) {
      $this->value_options[$list->name] = $list->name;
    }
  }
}
