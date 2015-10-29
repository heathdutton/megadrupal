<?php

/**
 * @file
 * Views handler for Match module
 */

class match_handler_argument_user_uid extends views_handler_argument_user_uid {
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $this->query->ensure_table('matches');

    $entity_types = match_entity_types_list();
    $entity_types = array_keys($entity_types);

    $account = user_load($this->argument);

    $sql = '';
    foreach ($entity_types as $entity_type) {
      $ids = match_participants_full_list($entity_type, $account);
      $string = implode(', ', $ids);
      if ($sql != '') {
        $sql .= ' OR ';
      }
      $sql .= "(matches.entity_type = '$entity_type' AND $this->table_alias.$this->real_field IN($string))";
    }

    $this->query->add_where_expression(0, $sql);
  }
}
