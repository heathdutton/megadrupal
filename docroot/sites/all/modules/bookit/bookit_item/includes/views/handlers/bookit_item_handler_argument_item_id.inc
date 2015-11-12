<?php

/**
 * Argument handler to display bookable item titles in View using item arguments.
 */
class bookit_item_handler_argument_item_id extends views_handler_argument_numeric {
  function title_query() {
    $titles = array();
    $result = db_select('bookit_item', 'b')
      ->fields('b', array('title'))
      ->condition('b.item_id', $this->value)
      ->execute();

    foreach ($result as $bookit_item) {
      $titles[] = check_plain($bookit_item->title);
    }

    return $titles;
  }
}
