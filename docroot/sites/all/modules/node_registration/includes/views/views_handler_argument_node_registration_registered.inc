<?php
/**
 * @file
 * Provide not registered users argument handler.
 */

/**
 * Argument handler to accept a user id.
 */
class views_handler_argument_node_registration_registered extends views_handler_argument_numeric {
  function query($group_by = FALSE) {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    $results = db_query("SELECT nr.uid FROM {node_registration} nr WHERE nr.nid IN (:nids) AND nr.cancelled = 0", array(':nids' => $this->value));
    $uids = array();
    foreach ($results as $result) {
      $uids[] = $result->uid;
    }

    $placeholder = $this->placeholder();
    $null_check = empty($this->options['not']) ? '' : "OR $this->table_alias.$this->real_field IS NULL";

    if (count($uids) > 1) {
      $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
      $this->query->add_where_expression(0, "$this->table_alias.$this->real_field $operator($placeholder) $null_check", array($placeholder => $uids));
    }
    else {
      $operator = empty($this->options['not']) ? '=' : '!=';
      $this->query->add_where_expression(0, "$this->table_alias.$this->real_field $operator $placeholder $null_check", array($placeholder => $uids));
    }
  }
}

