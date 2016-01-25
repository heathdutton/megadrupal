<?php

/**
 * @file
 * Provide abtest test_id argument handler.
 */

/**
 * Argument handler to accept a test_id.
 */
class abtest_views_handler_argument_test_id extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the node.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT test_name FROM {abtest} WHERE test_id IN (:test_ids)", array(':test_ids' => $this->value));
    foreach ($result as $obj) {
      $titles[] = check_plain($obj->test_name);
    }
    return $titles;
  }
}

