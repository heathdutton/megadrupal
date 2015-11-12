<?php

/**
 * @file
 * Contains CRUD operations for SimpleCrop module.
 */

/**
 * Create/update image crop data.
 *
 * @param $uri
 *   URI of image.
 *
 * @param $data
 *   Data that contains crop area.
 */
function simplecrop_crop_save($uri, $data) {

  // Create crop object with current info.
  $crop = new stdClass();
  $crop->uri = $uri;
  $crop->data = $data;
  $crop->original = simplecrop_crop_load($uri);

  // Save crop data into the database.
  db_merge('simplecrop')
    ->key(array('uri' => $crop->uri))
    ->fields(array('data' => serialize($crop->data)))
    ->execute();

  // Invoke update or insert hook for saved crop data.
  $op = !empty($crop->original) ? 'update' : 'insert';
  module_invoke_all('simplecrop_crop_' . $op, $crop);
}

/**
 * Load crop for image by uri.
 *
 * @param $uri
 *   URI of image.
 *
 * @param bool $reset
 *   Indicates whether to reset static cache.
 *
 * @return mixed
 */
function simplecrop_crop_load($uri, $reset = FALSE) {

  $cache = &drupal_static(__FUNCTION__);
  if (!isset($cache[$uri]) || $reset) {

    $cache[$uri] = db_select('simplecrop')
      ->fields('simplecrop')
      ->condition('uri', $uri)
      ->execute()
      ->fetchObject();

    if (!empty($cache[$uri])) {

      // Crop data stores serialized so we have to unserialize it before using.
      $cache[$uri]->data = unserialize($cache[$uri]->data);

      // Invoke hook to indicate that crop object has been loaded.
      module_invoke_all('simplecrop_crop_load', $cache[$uri]);
    }
  }

  return $cache[$uri];
}

/**
 * Delete image crop by file ID.
 *
 * @param $uri
 *   URI of image.
 */
function simplecrop_crop_delete($uri) {

  // Load crop before deletion to ensure that it exists.
  $crop = simplecrop_crop_load($uri);
  if (!empty($crop)) {

    // Execute hook before crop has been deleted.
    module_invoke_all('simplecrop_crop_delete', $crop);

    // Delete crop from table by its uri.
    db_delete('simplecrop')
      ->condition('uri', $crop->uri)
      ->execute();
  }
}
