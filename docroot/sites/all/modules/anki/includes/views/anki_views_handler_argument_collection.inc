<?php
/**
 * @file
 * Contains anki_views_handler_argument_collection class.
 */

/**
 * Views argument handler for getting the Anki collection name.
 */
class anki_views_handler_argument_collection extends views_handler_argument {
  // @todo: This needs to validate the argument! If not, this could allow someone to create loads
  // of arbitrary collections for non-existant users..

  function query() {
    // Set the collection to query from.
    $this->query->setCollection($this->argument);
  }
}
