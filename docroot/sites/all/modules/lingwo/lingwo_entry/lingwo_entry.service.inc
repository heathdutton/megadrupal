<?php

/**
 * @file
 * Contains all of our service callbacks
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function lingwo_entry_search_service($text, $options = array()) {
  return LingwoEntry::search($text, $options);
}

