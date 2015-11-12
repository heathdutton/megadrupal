<?php

/**
 * @file
 * Embed Import module API.
 */

/**
 * Implements hook_embed_import_alter().
 *
 * @param array $tags
 *  Multidimensional array with tags sorted by regions.
 */
function hook_embed_import_alter(&$tags) {
  // Don't render tags on admin pages.
  if (agr(0) == 'admin') {
    unset($tags);
  }
}
