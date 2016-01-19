<?php

/**
 * @file
 * Sort handler for ordering by comment number.
 *
 * @ingroup views_sort_handlers
 */

/**
 * Views field handler for comment number sorting.
 */
class views_handler_sort_comment_easy_reply_comment_number extends views_handler_sort_comment_thread {
  /**
   * Add a thread sorting to view's query.
   */
  function query() {
    $this->ensure_my_table();
    $this->real_field = 'thread';
    $alias = $this->table_alias . '_' . $this->real_field . $this->options['order'];
    $this->query->add_orderby(NULL, "SUBSTRING({$this->table_alias}.{$this->real_field}, 1, (LENGTH({$this->table_alias}.{$this->real_field}) - 1))", $this->options['order'], $alias);
  }
}
