<?php

/**
 * Argument handler to display booking option titles in View using option arguments.
 */
class bookit_option_handler_argument_option_id extends views_handler_argument_numeric {
  function title_query() {
    $titles = array();
    $result = db_select('bookit_option', 'b')
      ->fields('b', array('title'))
      ->condition('b.option_id', $this->value)
      ->execute();

    foreach ($result as $bookit_option) {
      $titles[] = check_plain($bookit_option->title);
    }

    return $titles;
  }
}
