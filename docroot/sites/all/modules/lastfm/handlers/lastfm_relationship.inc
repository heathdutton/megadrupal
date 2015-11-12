<?php

class lastfm_relationship extends views_handler_relationship {
  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $right_table = $this->definition['base'];
    $method = $this->definition['lastfm']['method'];
    $arg = $this->definition['lastfm']['arg'];

    $this->query->add_relationship($right_table, $method, $arg);
  }
}
