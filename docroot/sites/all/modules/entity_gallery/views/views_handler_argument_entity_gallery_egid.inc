<?php

/**
 * @file
 * Provide entity gallery egid argument handler.
 */

/**
 * Argument handler to accept an entity gallery id.
 */
class views_handler_argument_entity_gallery_egid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the entity gallery.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT eg.title FROM {entity_gallery} eg WHERE eg.egid IN (:egids)", array(':egids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }
}
