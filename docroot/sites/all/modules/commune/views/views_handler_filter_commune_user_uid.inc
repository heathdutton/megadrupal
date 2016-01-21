<?php

/**
 * @file
 * Definition of views_handler_filter_commune_user_uid.
 */

/**
 * Filter handler to accept a user id to check for nodes that user was referred to directly or @mention.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_commune_user_uid extends views_handler_filter_user_name {
  function query() {
    $this->ensure_my_table();

    $subselect = db_select('commune_context', 'c');
    $subselect->addField('c', 'context_id');
    $subselect->condition('c.context_id', $this->value, $this->operator);
    $subselect->where("c.pid = $this->table_alias.nid");

    $condition = db_or()
      ->condition("$this->table_alias.uid", $this->value, $this->operator)
      ->exists($subselect);

    $this->query->add_where($this->options['group'], $condition);
  }
}
