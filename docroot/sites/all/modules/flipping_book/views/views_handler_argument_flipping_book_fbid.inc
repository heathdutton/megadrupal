<?php

/**
 * @file
 * Provide flipping book fbid argument handler.
 */

/**
 * Argument handler to accept a flipping_book id.
 */
class views_handler_argument_flipping_book_fbid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the flipping_book.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT f.title FROM {flipping_book} f WHERE f.fbid IN (:fbids)", array(':fbids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }
}
