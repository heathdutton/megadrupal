<?php

/**
 * @file
 * Provide entity gallery vid argument handler.
 */

/**
 * Argument handler to accept an entity gallery revision id.
 */
class views_handler_argument_entity_gallery_vid extends views_handler_argument_numeric {
  // No constructor is necessary.

  /**
   * Override the behavior of title(). Get the title of the revision.
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT eg.title FROM {entity_gallery_revision} eg WHERE eg.vid IN (:vids)", array(':vids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }
}
